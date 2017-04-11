#include "GitModule.h"

GitModule::GitModule(Sqlite *db) : SqliteModule("git"), db(db) {
  std::cout << "GitModule()" << std::endl;
}
GitModule::~GitModule() {
  std::cout << "~GitModule()" << std::endl;
  git_repository_free(repository);
}

std::unique_ptr<SqliteVirtualTable>
GitModule::create(std::vector<std::string> args) {
  for (auto arg : args) {
    std::cout << "Create argv=" << arg << std::endl;
  }
  if (!open(args[args.size() - 1])) {
    std::cerr << "Unable to open repo " << args[args.size() - 1] << std::endl;
    return {};
  }
  db->declareVTab(R"SQL(CREATE TABLE commits(
sha TEXT,
author_name TEXT,
author_email TEXT,
message TEXT)
)SQL");
  return std::make_unique<GitVirtualTable>(repository);
}

std::unique_ptr<SqliteVirtualTable>
GitModule::connect(std::vector<std::string> args) {
  /// XXX: Not sure if this is needed at all
  return std::make_unique<GitVirtualTable>(repository);
}

/**
 * Open git repo
 */
bool GitModule::open(std::string path) {
  if (repository != nullptr) {
    return false;
  }
  std::cout << "Init git repository: " << path << std::endl;
  int err;
  if ((err = git_repository_open(&repository, path.c_str())) < 0) {
    const auto e = giterr_last();
    std::cerr << "Unable to open git repository " << err << "/" << e->klass
              << ": " << e->message << std::endl;
    return false;
  }
  return true;
}
