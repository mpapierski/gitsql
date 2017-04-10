#include "SqliteModule.h"

SqliteModule::SqliteModule(std::string name, int version)
    : name{std::move(name)} {
  mod.iVersion = version;
  mod.xCreate = &xCreate;
  mod.xConnect = &xConnect;
  mod.xBestIndex = &xBestIndex;
  mod.xDisconnect = &xDisconnect;
  mod.xDestroy = &xDestroy;
  mod.xOpen = &xOpen;
  mod.xClose = &xClose;
  mod.xFilter = &xFilter;
  mod.xNext = &xNext;
  mod.xEof = &xEof;
  mod.xColumn = &xColumn;
  mod.xRowid = &xRowid;
  mod.xUpdate = &xUpdate;
  mod.xBegin = &xBegin;
  mod.xSync = &xSync;
  mod.xCommit = &xCommit;
  mod.xRollback = &xRollback;
  mod.xFindFunction = &xFindFunction;
  mod.xRename = &xRename;
  mod.xSavepoint = &xSavepoint;
  mod.xRelease = &xRelease;
  mod.xRollbackTo = &xRollbackTo;
}

int SqliteModule::xCreate(sqlite3 *db, void *pAux, int argc,
                          const char *const *argv, sqlite3_vtab **ppVTab,
                          char **err) {
  auto sqliteModule = static_cast<SqliteModule *>(pAux);
  std::vector<std::string> args(argv, argv + argc);
  sqliteModule->vtab = sqliteModule->create(std::move(args));
  if (sqliteModule->vtab == nullptr) {
    std::cerr << "Unable to create vtab" << std::endl;
    return SQLITE_ERROR;
  }
  *ppVTab = sqliteModule->vtab.get();
  return SQLITE_OK;
}

int SqliteModule::xConnect(sqlite3 *db, void *pAux, int argc,
                           const char *const *argv, sqlite3_vtab **ppVTab,
                           char **err) {
  auto sqliteModule = static_cast<SqliteModule *>(pAux);
  std::vector<std::string> args(argv, argv + argc);
  sqliteModule->vtab = sqliteModule->connect(std::move(args));
  if (sqliteModule->vtab == nullptr) {
    std::cerr << "Unable to create vtab" << std::endl;
    return SQLITE_ERROR;
  }
  *ppVTab = sqliteModule->vtab.get();
  return SQLITE_OK;
}

int SqliteModule::xBestIndex(sqlite3_vtab *pVTab, sqlite3_index_info *info) {
  std::cout << "xBestIndex" << std::endl;
  auto virtualTable = static_cast<SqliteVirtualTable *>(pVTab);
  return virtualTable->bestIndex(info);
}

int SqliteModule::xDisconnect(sqlite3_vtab *pVTab) {
  std::cout << "xDisconnect" << std::endl;
  auto virtualTable = static_cast<SqliteVirtualTable *>(pVTab);
  return virtualTable->disconnect();
}

int SqliteModule::xDestroy(sqlite3_vtab *pVTab) {
  std::cout << "xDestroy" << std::endl;
  auto virtualTable = static_cast<SqliteVirtualTable *>(pVTab);
  delete virtualTable;
  return SQLITE_OK;
}

int SqliteModule::xOpen(sqlite3_vtab *pVTab, sqlite3_vtab_cursor **ppCursor) {
  auto sqliteVTab = static_cast<SqliteVirtualTable *>(pVTab);
  auto cursor = sqliteVTab->createCursor();
  *ppCursor = cursor.release();
  return SQLITE_OK;
}

int SqliteModule::xClose(sqlite3_vtab_cursor *cursor) {
  std::cout << "xClose" << std::endl;
  auto sqliteVTabCursor = static_cast<SqliteVirtualTableCursor *>(cursor);
  delete sqliteVTabCursor;
  return SQLITE_OK;
}

int SqliteModule::xFilter(sqlite3_vtab_cursor *cursor, int idxNum,
                          const char *idxStr, int argc, sqlite3_value **argv) {
  std::cout << "xFilter" << std::endl;
  auto sqliteVTabCursor = static_cast<SqliteVirtualTableCursor *>(cursor);
  return sqliteVTabCursor->filter(idxNum, idxStr, argc, argv);
}

int SqliteModule::xNext(sqlite3_vtab_cursor *cursor) {
  std::cout << "xNext" << std::endl;
  auto sqliteVTabCursor = static_cast<SqliteVirtualTableCursor *>(cursor);
  return sqliteVTabCursor->next();
}

int SqliteModule::xEof(sqlite3_vtab_cursor *cursor) {
  std::cout << "xEof" << std::endl;
  auto sqliteVTabCursor = static_cast<SqliteVirtualTableCursor *>(cursor);
  return sqliteVTabCursor->eof();
}

int SqliteModule::xColumn(sqlite3_vtab_cursor *cursor, sqlite3_context *context,
                          int N) {
  std::cout << "xColumn" << std::endl;
  auto sqliteVTabCursor = static_cast<SqliteVirtualTableCursor *>(cursor);
  return sqliteVTabCursor->column(context, N);
}

int SqliteModule::xRowid(sqlite3_vtab_cursor *cursor, sqlite3_int64 *pRowid) {
  std::cout << "xRowid" << std::endl;
  auto sqliteVTabCursor = static_cast<SqliteVirtualTableCursor *>(cursor);
  *pRowid = sqliteVTabCursor->rowId();
  return SQLITE_OK;
}

int SqliteModule::xUpdate(sqlite3_vtab *, int, sqlite3_value **,
                          sqlite3_int64 *) {
  std::cout << "xUpdate" << std::endl;
  return SQLITE_OK;
}

int SqliteModule::xBegin(sqlite3_vtab *pVTab) {
  std::cout << "xBegin" << std::endl;
  return SQLITE_OK;
}

int SqliteModule::xSync(sqlite3_vtab *pVTab) {
  std::cout << "xSync" << std::endl;
  return SQLITE_OK;
}
int SqliteModule::xCommit(sqlite3_vtab *pVTab) {
  std::cout << "xCommit" << std::endl;
  return SQLITE_OK;
}
int SqliteModule::xRollback(sqlite3_vtab *pVTab) {
  std::cout << "xRollback" << std::endl;
  return SQLITE_OK;
}
int SqliteModule::xFindFunction(
    sqlite3_vtab *pVtab, int nArg, const char *zName,
    void (**pxFunc)(sqlite3_context *, int, sqlite3_value **), void **ppArg) {
  std::cout << "xFindFunction" << std::endl;
  return SQLITE_OK;
}
int SqliteModule::xRename(sqlite3_vtab *pVtab, const char *zNew) {
  std::cout << "xRename" << std::endl;
  return SQLITE_OK;
}
/* The methods above are in version 1 of the sqlite_module object. Those
** below are for version 2 and greater. */
int SqliteModule::xSavepoint(sqlite3_vtab *pVTab, int) {
  std::cout << "xSavepoint" << std::endl;
  return SQLITE_OK;
}
int SqliteModule::xRelease(sqlite3_vtab *pVTab, int) {
  std::cout << "xRelease" << std::endl;
  return SQLITE_OK;
}
int SqliteModule::xRollbackTo(sqlite3_vtab *pVTab, int) {
  std::cout << "xRollbackTo" << std::endl;
  return SQLITE_OK;
}