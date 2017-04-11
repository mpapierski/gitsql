
#include "GitVirtualTable.h"
GitVirtualTable::GitVirtualTable(git_repository *repository)
    : repository(repository) {}

std::unique_ptr<SqliteVirtualTableCursor> GitVirtualTable::createCursor() {
  return std::make_unique<GitVirtualTableCursor>(repository);
}

int GitVirtualTable::bestIndex(sqlite3_index_info *info) {
  return SQLITE_OK;
}

int GitVirtualTable::disconnect() {
  return SQLITE_OK;
}