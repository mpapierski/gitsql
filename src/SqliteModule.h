#pragma once

#include "SqliteVirtualTable.h"
#include <iostream>
#include <memory>
#include <sqlite3.h>
#include <string>
#include <vector>

class SqliteModule {
public:
  SqliteModule(std::string name, int version = 1);
  virtual ~SqliteModule() = default;
  std::string name;
  const std::string &getName() const { return name; }

  virtual std::unique_ptr<SqliteVirtualTable>
  create(std::vector<std::string> args) = 0;
  virtual std::unique_ptr<SqliteVirtualTable>
  connect(std::vector<std::string> args) = 0;

  sqlite3_module mod{};

private:
  std::unique_ptr<SqliteVirtualTable> vtab;
  static int xCreate(sqlite3 *db, void *pAux, int argc, const char *const *argv,
                     sqlite3_vtab **ppVTab, char **err);
  static int xConnect(sqlite3 *db, void *pAux, int argc,
                      const char *const *argv, sqlite3_vtab **ppVTab,
                      char **err);
  static int xBestIndex(sqlite3_vtab *pVTab, sqlite3_index_info *info);
  static int xDisconnect(sqlite3_vtab *pVTab);
  static int xDestroy(sqlite3_vtab *pVTab);
  static int xOpen(sqlite3_vtab *pVTab, sqlite3_vtab_cursor **ppCursor);
  static int xClose(sqlite3_vtab_cursor *cursor);
  static int xFilter(sqlite3_vtab_cursor *cursor, int idxNum,
                     const char *idxStr, int argc, sqlite3_value **argv);
  static int xNext(sqlite3_vtab_cursor *cursor);
  static int xEof(sqlite3_vtab_cursor *cursor);
  static int xColumn(sqlite3_vtab_cursor *cursor, sqlite3_context *context,
                     int N);
  static int xRowid(sqlite3_vtab_cursor *cursor, sqlite3_int64 *pRowid);
  static int xUpdate(sqlite3_vtab *, int, sqlite3_value **, sqlite3_int64 *);
  static int xBegin(sqlite3_vtab *pVTab);
  static int xSync(sqlite3_vtab *pVTab);
  static int xCommit(sqlite3_vtab *pVTab);
  static int xRollback(sqlite3_vtab *pVTab);
  static int xFindFunction(sqlite3_vtab *pVtab, int nArg, const char *zName,
                           void (**pxFunc)(sqlite3_context *, int,
                                           sqlite3_value **),
                           void **ppArg);
  static int xRename(sqlite3_vtab *pVtab, const char *zNew);
  /* The methods above are in version 1 of the sqlite_module object. Those
  ** below are for version 2 and greater. */
  static int xSavepoint(sqlite3_vtab *pVTab, int);
  static int xRelease(sqlite3_vtab *pVTab, int);
  static int xRollbackTo(sqlite3_vtab *pVTab, int);
};
