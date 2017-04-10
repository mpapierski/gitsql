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