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
  int rc;
  auto sqliteModulePtr = sqliteModule.release();
  if ((rc = sqlite3_create_module_v2(
           db, /* SQLite connection to register module with */
           sqliteModulePtr->getName().c_str(), /* Name of the module */
           &sqliteModulePtr->mod,              /* Methods for the module */
           sqliteModulePtr, /* Client data for xCreate/xConnect */
           [](void *ptr) {
             std::cout << "cleanup(" << ptr << ")" << std::endl;
             delete static_cast<SqliteModule *>(ptr);
           })) != SQLITE_OK) {
    throw std::system_error(rc, sqlite_error_category(),
                            "sqlite3_create_module_v2");
  }
}

void Sqlite::fetchRows(Query query, Callback callback) {
  int rc;
  char *err;
  auto cCallback = [](void *ptr, int argc, char **vals, char **cols) {
    auto result = (*(static_cast<Callback *>(ptr)))(argc, vals, cols);
    if (result) {
      return 0;
    } else {
      // abort
      return 1;
    }
  };

  if ((rc = sqlite3_exec(db, query.get(), cCallback, &callback, &err)) !=
      SQLITE_OK) {
    std::cerr << err << std::endl;
    throw std::system_error(rc, sqlite_error_category(), "sqlite3_exec");
  }
}

void Sqlite::declareVTab(std::string statement) {
  int rc;
  if ((rc = sqlite3_declare_vtab(db, statement.c_str())) != SQLITE_OK) {
    throw std::system_error(rc, sqlite_error_category(), "sqlite3_exec");
  }
}