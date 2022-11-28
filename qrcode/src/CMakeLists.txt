add_executable(run run.cc)

target_link_libraries(run PUBLIC qrcode)

add_subdirectory(qrcode)
