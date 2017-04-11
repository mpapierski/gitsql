#pragma once

#include "SqliteVirtualTableCursor.h"
#include <cassert>
#include <git2.h>
#include <iostream>
#include <memory>
#include <vector>

class GitVirtualTableCursor : public SqliteVirtualTableCursor {
public:
  GitVirtualTableCursor(git_repository *repository);
  virtual ~GitVirtualTableCursor();
  int filter(int, const char *, int, sqlite3_value **) override;
  int next() override;
  int column(sqlite3_context *context, int N) override;
  bool eof() override;
  int rowId() override;

private:
  git_repository *repository;
  git_revwalk *walker{nullptr};
  std::vector<std::string> values;
  int currentRowId{0};
};