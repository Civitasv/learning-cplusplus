include(GoogleTest)
add_executable(RePlexGTest RePlexTest.cpp)

target_link_libraries(RePlexGTest
    PUBLIC
    gtest_main)

target_include_directories(RePlexGTest PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
    )

gtest_discover_tests(RePlexGTest)