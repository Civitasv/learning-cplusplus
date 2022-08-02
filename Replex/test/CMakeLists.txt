add_library(RePlexTest SHARED Test.cpp)

target_include_directories(RePlexTest PUBLIC ${PROJECT_SOURCE_DIR}/test/pub)

add_subdirectory(pub)