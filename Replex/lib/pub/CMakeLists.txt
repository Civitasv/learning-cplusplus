if(WIN32)
    add_subdirectory(windows)
endif(WIN32)

if(UNIX)
    add_subdirectory(linux)
endif(UNIX)