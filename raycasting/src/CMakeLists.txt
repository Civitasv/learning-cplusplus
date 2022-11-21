add_executable(raycasting run.cc app.cc ray_window.cc)

target_include_directories(raycasting PUBLIC
	${CMAKE_SOURCE_DIR}/deps/raylib/include
	${CMAKE_SOURCE_DIR}/include)

target_link_libraries(raycasting PUBLIC
	${CMAKE_SOURCE_DIR}/deps/raylib/lib/raylibdll.lib)
