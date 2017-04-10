add_executable (gitsql
    Sqlite.cpp
    Sqlite.h
    SqliteModule.cpp
    SqliteModule.h
    SqliteVirtualTable.h   
    SqliteVirtualTableCursor.h
    main.cpp
    )
target_include_directories (gitsql
    PRIVATE ${SQLITE3_INCLUDE_DIRS}
    )
target_link_libraries (gitsql
    ${SQLITE3_LIBRARIES}
    )