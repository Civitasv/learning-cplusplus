add_executable(test test.cc)

target_include_directories(test PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>)