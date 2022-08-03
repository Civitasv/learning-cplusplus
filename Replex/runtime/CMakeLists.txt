add_executable(RePlexExe Main.cpp)

target_include_directories(RePlexExe PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
    )
