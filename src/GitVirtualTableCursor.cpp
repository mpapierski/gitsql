#include "GitVirtualTableCursor.h"

GitVirtualTableCursor::GitVirtualTableCursor(git_repository *repository)
    : repository(repository) {
  assert(repository != nullptr);
  git_revwalk_new(&walker, repository);
  git_revwalk_sorting(walker, GIT_SORT_TOPOLOGICAL);
  git_revwalk_push_head(walker);
}
GitVirtualTableCursor::~GitVirtualTableCursor() {
  std::cout << "~GitVirtualTableCursor" << std::endl;
  git_revwalk_free(walker);
}
int GitVirtualTableCursor::filter(int, const char *, int, sqlite3_value **) {
  return next();
}
int GitVirtualTableCursor::next() {

  int err;
  git_oid oid;
  if ((err = git_revwalk_next(&oid, walker)) != 0) {
    const auto e = giterr_last();
    if (!e) {
      currentRowId = -1;
      return SQLITE_OK;
    }

    std::cerr << "git revwalk next error: " << err << ":" << e->message
              << std::endl;
    return SQLITE_ERROR;
  }

  git_commit *commit = nullptr;
  if (git_commit_lookup(&commit, repository, &oid)) {
    std::cerr << "Failed to lookup the next object" << std::endl;
    return SQLITE_ERROR;
  }

  values.clear();

  // get git sha
  char out[41] = {};
  git_oid_fmt(out, &oid);
  values.push_back(out);
  auto author = git_commit_author(commit);
  values.push_back(author->name);
  values.push_back(author->email);
  values.push_back(git_commit_message(commit));

  git_commit_free(commit);

  currentRowId++;
  return SQLITE_OK;
}
int GitVirtualTableCursor::column(sqlite3_context *context, int N) {
  const auto &value = values.at(N);

  sqlite3_result_text(context, value.c_str(), -1, SQLITE_STATIC);
  return SQLITE_OK;
}
bool GitVirtualTableCursor::eof() { return currentRowId == -1; }
int GitVirtualTableCursor::rowId() { return currentRowId; }