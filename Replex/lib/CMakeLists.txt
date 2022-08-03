add_library(RePlexLib SHARED "")

set_target_properties(RePlexLib PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(RePlexLib PUBLIC ${PROJECT_SOURCE_DIR}/lib/pub/linux)
add_subdirectory(pub)