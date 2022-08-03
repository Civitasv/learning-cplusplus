add_library(RePlexTest SHARED Test.cpp)

add_subdirectory(pub)

target_include_directories(RePlexTest PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
    )
