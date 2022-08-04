add_library(RePlexTest SHARED Test.cpp)

target_include_directories(RePlexTest PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
    )

enable_testing()
add_subdirectory(pub)
add_subdirectory(gtest)
