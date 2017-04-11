#pragma once

#include "GitVirtualTable.h"
#include "Sqlite.h"
#include "SqliteModule.h"
#include <git2.h>
#include <memory>

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
   */
  bool open(std::string path);
  git_repository *repository{nullptr};
  Sqlite *db;
};