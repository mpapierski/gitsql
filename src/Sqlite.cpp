#include "Sqlite.h"

Sqlite::Sqlite(std::string database) {
  int rc;
  if ((rc = sqlite3_open(database.c_str(), &db)) != SQLITE_OK) {
    throw std::system_error(rc, sqlite_error_category(), "sqlite3_open");
  }
}

Sqlite::~Sqlite() { sqlite3_close(db); }

void Sqlite::exec(Query query) {
  int rc;
  char *err;
  if ((rc = sqlite3_exec(db, query.get(), nullptr, nullptr, &err)) !=
      SQLITE_OK) {
    std::cerr << err << std::endl;
    sqlite3_free(err);
    throw std::system_error(rc, sqlite_error_category(), "sqlite3_exec");
  }
}

void Sqlite::createModule(std::unique_ptr<SqliteModule> sqliteModule) {
  assert(db != nullptr);
  // Here instance of sqliteModule is released as sqlite
  // will become owner of this memory. We pass additional deleter
  // to the function call that will delete this object when
  // it is not required anymore.
  auto sqliteModulePtr = sqliteModule.release();
  int rc;  
  if ((rc = sqlite3_create_module_v2(
           db, /* SQLite connection to register module with */
           sqliteModulePtr->getName().c_str(), /* Name of the module */
           &sqliteModulePtr->mod,              /* Methods for the module */
           sqliteModulePtr, /* Client data for xCreate/xConnect */
           [](void *ptr) {
             // Deletes instance of module that is managed by sqlite
             delete static_cast<SqliteModule *>(ptr);
           })) != SQLITE_OK) {
    throw std::system_error(rc, sqlite_error_category(),
                            "sqlite3_create_module_v2");
  }
}

void Sqlite::fetchRows(Query query, Callback callback) {
  int rc;
  // C callback used to call C++ callback passed as first argument
  auto cCallback = [](void *ptr, int argc, char **vals, char **cols) {
    auto result = (*(static_cast<Callback *>(ptr)))(argc, vals, cols);
    // TODO: Probably no need for this
    if (result) {
      return 0;
    } else {
      // Abort
      return 1;
    }
  };
  char *err;
  if ((rc = sqlite3_exec(db, query.get(), cCallback, &callback, &err)) !=
      SQLITE_OK) {
    std::cerr << err << std::endl;
    sqlite3_free(err);
    throw std::system_error(rc, sqlite_error_category(), "sqlite3_exec");
  }
}

void Sqlite::declareVTab(std::string statement) {
  int rc;
  if ((rc = sqlite3_declare_vtab(db, statement.c_str())) != SQLITE_OK) {
    throw std::system_error(rc, sqlite_error_category(), "sqlite3_declare_vtab");
  }
}