cmake_minimum_required (VERSION 2.8.7)
project (git_sql LANGUAGES C CXX)

set (CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules")
set (CMAKE_CXX_STANDARD 14)

find_package (SQLite3 REQUIRED)
find_package (Libgit2 REQUIRED)

add_subdirectory (src)