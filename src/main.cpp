#include "GitModule.h"
#include "Sqlite.h"
#include <iostream>

bool displayResults(int argc, char **argv, char **azColName) {
  for (int i = 0; i < argc; i++) {
    std::cout << "Result: " << azColName[i] << "="
              << "\"" << argv[i] << "\"" << std::endl;
  }
  return true;
}

int main(int argc, char *argv[]) try {
  if (argc < 2) {
    std::cerr << argv[0] << " [git repo]" << std::endl;
    return 1;
  }
  Sqlite db{":memory:"};
  db.createModule(std::make_unique<GitModule>(&db));
  db.exec(
      db.formatQuery("CREATE VIRTUAL TABLE commits USING git(%q)", argv[1]));
  db.fetchRows(db.formatQuery("SELECT * FROM commits"), &displayResults);
} catch (std::exception &e) {
  std::cerr << e.what() << std::endl;
  return 1;
}