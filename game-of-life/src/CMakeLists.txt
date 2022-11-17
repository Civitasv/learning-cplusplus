add_executable(game-of-life run.cc app.cc ray_window.cc)

target_include_directories(game-of-life PUBLIC
	${CMAKE_SOURCE_DIR}/deps/raylib/include
	${CMAKE_SOURCE_DIR}/include)

target_link_libraries(game-of-life PUBLIC
	${CMAKE_SOURCE_DIR}/deps/raylib/lib/raylibdll.lib)
