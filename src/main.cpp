#include "Sqlite.h"
#include <iostream>

int main(int argc, char *argv[]) try {
  if (argc < 2) {
    std::cerr << argv[0] << " [git repo]" << std::endl;
    return 1;
  }
  Sqlite db{":memory:"};
} catch (std::exception &e) {
  std::cerr << e.what() << std::endl;
  return 1;
}