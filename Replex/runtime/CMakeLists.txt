add_executable(RePlexExe Main.cpp)

target_link_libraries(RePlexExe RePlexLib)

target_include_directories(RePlexExe PUBLIC ${PROJECT_SOURCE_DIR}/test/pub)