add_executable(fourier_transform run.cc app.cc ray_window.cc fourier.cc)

target_include_directories(fourier_transform PUBLIC
	${CMAKE_SOURCE_DIR}/deps/raylib/include
	${CMAKE_SOURCE_DIR}/include
	${CMAKE_SOURCE_DIR}/deps/svg)

target_link_libraries(fourier_transform PUBLIC
	${CMAKE_SOURCE_DIR}/deps/raylib/lib/libraylib.a)
