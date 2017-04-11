
#include "GitVirtualTable.h"
GitVirtualTable::GitVirtualTable(git_repository *repository)
    : repository(repository) {}

std::unique_ptr<SqliteVirtualTableCursor> GitVirtualTable::createCursor() {
  std::cout << "Create cursor" << std::endl;

  return std::make_unique<GitVirtualTableCursor>(repository);
}

int GitVirtualTable::bestIndex(sqlite3_index_info *info) {
  std::cout << "GitVTable::bestIndex(" << info << ")" << std::endl;
  return SQLITE_OK;
}
int GitVirtualTable::disconnect() {
  std::cout << "GitVTable::disconnect()" << std::endl;
  return SQLITE_OK;
}