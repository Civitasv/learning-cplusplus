add_executable(easygame run.cc)

target_include_directories(easygame PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>)
