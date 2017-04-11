#pragma once

#include "SqliteModule.h"
#include <iostream>
#include <memory>
#include <cassert>
#include <sqlite3.h>
#include <system_error>

struct sqlite_error_category : std::error_category {
  const char *name() const noexcept override { return "sqlite3"; }
  std::string message(int rc) const override { return ::sqlite3_errstr(rc); }
};

struct Sqlite3Free {
  void operator()(void *ptr) { sqlite3_free(ptr); }
};

/**
 * Wrapper for a Sqlite database handle
 */
class Sqlite {
public:
  /**
   * Constructs database object and connects.
   * @param database A valid address such as :memory:
   */
  Sqlite(std::string database);
  ~Sqlite();
  using Query = std::unique_ptr<char, Sqlite3Free>;
  /**
   * Formats a SQL query with provided params
   * @param format Query format
   * @param t Query params
   * @return A query object with formatted query
   */
  template <typename... T> Query formatQuery(std::string format, T... t) {
    char *query = sqlite3_mprintf(format.c_str(), std::forward<T>(t)...);
    return Query{query};
  }
  /**
   * Executes single query and does not expect any results
   * @param query Query object created with Sqlite::formatQuery
   */
  void exec(Query query);
  /**
   * Create a sqlite module. Pass an instance to SqliteModule
   * @param sqliteModule Sqlite module
   */
  void createModule(std::unique_ptr<SqliteModule> sqliteModule);
  /// Wrapper for sqlite callback 
  using Callback = std::function<bool(int, char **, char **)>;
  /**
   * Execute a query and expect results
   * @param query Query object created with Sqlite::formatQuery
   * @param callback Callback
   */
  void fetchRows(Query query, Callback callback);
  /**
   * Declare a new virtual table.
   * @param statement A valid SQL create table statement
   */
  void declareVTab(std::string statement);

private:
  /// This is a handle to sqlite3 database
  sqlite3 *db{nullptr};
};