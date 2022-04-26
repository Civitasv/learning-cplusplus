cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD_REQUIRED True)

# set the project name
project(programming)

add_library(myLib "")

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

add_subdirectory(${PROJECT_SOURCE_DIR}/src)

target_include_directories(myLib PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_include_directories(execute PUBLIC ${PROJECT_SOURCE_DIR}/include)
