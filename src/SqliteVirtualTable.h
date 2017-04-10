#pragma once

#include "SqliteVirtualTableCursor.h"
#include <memory>
#include <sqlite3.h>

class SqliteVirtualTable : public sqlite3_vtab {
public:
  virtual ~SqliteVirtualTable() = default;
  virtual std::unique_ptr<SqliteVirtualTableCursor> createCursor() = 0;
  virtual int bestIndex(sqlite3_index_info *info) = 0;
  virtual int disconnect() = 0;
};
