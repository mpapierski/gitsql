add_executable (gitsql
    Sqlite.cpp
    Sqlite.h
    SqliteModule.cpp
    SqliteModule.h
    SqliteVirtualTable.h   
    SqliteVirtualTableCursor.h
    GitModule.cpp
    GitModule.h
    GitVirtualTable.cpp
    GitVirtualTable.h
    GitVirtualTableCursor.cpp
    GitVirtualTableCursor.h
    main.cpp
    )
target_include_directories (gitsql
    PRIVATE ${SQLITE3_INCLUDE_DIRS}
    PRIVATE ${LIBGIT2_INCLUDE_DIR}
    )
target_link_libraries (gitsql
    ${SQLITE3_LIBRARIES}
    ${LIBGIT2_LIBRARIES}
    )