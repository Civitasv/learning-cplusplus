add_executable(just_a_clock run.cc app.cc ray_window.cc)

target_include_directories(just_a_clock PUBLIC
	${CMAKE_SOURCE_DIR}/deps/raylib/include
	${CMAKE_SOURCE_DIR}/include)

target_link_libraries(just_a_clock PUBLIC
	${CMAKE_SOURCE_DIR}/deps/raylib/lib/raylibdll.lib)
