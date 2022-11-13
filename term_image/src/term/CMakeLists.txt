add_library(term_image term_image.cc)

set(OPENCV_DIR ${CMAKE_SOURCE_DIR}/deps/opencv)
message("PATH" ${OPENCV_DIR})

target_include_directories(term_image PUBLIC
    ${PROJECT_SOURCE_DIR}/include
    ${OPENCV_DIR}/include
)

target_link_libraries(term_image PUBLIC
    ${OPENCV_DIR}/x64/vc14/lib/opencv_world343d.lib)