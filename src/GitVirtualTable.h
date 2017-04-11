#pragma once

#include "GitVirtualTableCursor.h"
#include "SqliteVirtualTable.h"
#include "SqliteVirtualTableCursor.h"
#include <git2.h>
#include <iostream>
#include <sqlite3.h>

class GitVirtualTable : public SqliteVirtualTable {
public:
  GitVirtualTable(git_repository *repository);
  std::unique_ptr<SqliteVirtualTableCursor> createCursor() override;
  int bestIndex(sqlite3_index_info *info) override;
  int disconnect() override;

private:
  git_repository *repository;
};