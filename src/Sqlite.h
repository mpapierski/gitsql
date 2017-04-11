#pragma once

#include "SqliteModule.h"
#include <iostream>
#include <memory>
#include <sqlite3.h>
#include <system_error>

struct sqlite_error_category : std::error_category {
  const char *name() const noexcept override { return "sqlite3"; }
  std::string message(int rc) const override { return ::sqlite3_errstr(rc); }
};

struct Sqlite3Free {
  void operator()(void *ptr) { sqlite3_free(ptr); }
};

class Sqlite {
public:
  Sqlite(std::string database);
  ~Sqlite();
  using Query = std::unique_ptr<char, Sqlite3Free>;
  template <typename... T> Query formatQuery(std::string format, T... t) {
    char *query = sqlite3_mprintf(format.c_str(), std::forward<T>(t)...);
    return Query{query};
  }
  void exec(Query query);
  void createModule(std::unique_ptr<SqliteModule> sqliteModule);
  using Callback = std::function<bool(int, char **, char **)>;
  void fetchRows(Query query, Callback callback);
  void declareVTab(std::string statement);

private:
  sqlite3 *db{nullptr};
};