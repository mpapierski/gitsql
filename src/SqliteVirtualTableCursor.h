#pragma once

#include <sqlite3.h>

/**
 * This is a cursor implementation that does all the
 * processing of rows.
 */
class SqliteVirtualTableCursor : public sqlite3_vtab_cursor {
public:
  virtual ~SqliteVirtualTableCursor() = default;
  virtual int filter(int idxNum, const char *idxStr, int argc,
                     sqlite3_value **argv) = 0;
  virtual int next() = 0;
  virtual int column(sqlite3_context *context, int N) = 0;
  virtual bool eof() = 0;
  virtual int rowId() = 0;
};