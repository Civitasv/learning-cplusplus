add_executable(run run.cc)

target_link_libraries(run PUBLIC term_image)

add_subdirectory(term)
