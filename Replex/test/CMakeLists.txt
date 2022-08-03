add_library(RePlexTest SHARED Test.cpp)

target_include_directories(RePlexTest PUBLIC ${PROJECT_SOURCE_DIR}/test/pub)
target_include_directories(RePlexTest PUBLIC ${PROJECT_SOURCE_DIR}/lib/pub/linux)

add_subdirectory(pub)