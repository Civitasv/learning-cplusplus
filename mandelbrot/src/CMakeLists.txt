add_executable(mandelbrot run.cc app.cc ray_window.cc)

target_include_directories(mandelbrot PUBLIC
	${CMAKE_SOURCE_DIR}/deps/raylib/include
	${CMAKE_SOURCE_DIR}/include)

target_link_libraries(mandelbrot PUBLIC
	${CMAKE_SOURCE_DIR}/deps/raylib/lib/raylibdll.lib)
