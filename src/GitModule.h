#pragma once

#include "GitVirtualTable.h"
#include "Sqlite.h"
#include "SqliteModule.h"
#include <git2.h>
#include <memory>

/**
 * An implementation of sqlite modules meant to
 * provide SQL access to git commits.
 */
class GitModule : public SqliteModule {
public:
  GitModule(Sqlite *db);
  virtual ~GitModule();
  std::unique_ptr<SqliteVirtualTable>
  create(std::vector<std::string> args) override;
  std::unique_ptr<SqliteVirtualTable>
  connect(std::vector<std::string> args) override;
private:
  /**
   * Open git repo
   * @param path Path to git repository
   */
  bool open(std::string path);
  /// Git repository
  git_repository *repository{nullptr};
  /// An instance of Sqlite wrapper
  Sqlite *db;
};