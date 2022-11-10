add_library(term_image term_image.cc)

target_include_directories(term_image PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>)