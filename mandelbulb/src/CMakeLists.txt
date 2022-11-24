add_library(pgl STATIC 
	index_buffer.cc 
	log.cc 
	renderer.cc 
	shader.cc 
	texture.cc 
	vertex_array.cc 
	vertex_buffer.cc
	${CMAKE_SOURCE_DIR}/deps/GLAD/src/gl.c
	${CMAKE_SOURCE_DIR}/deps/stb_image/stb_image.cpp)

target_include_directories(pgl PUBLIC 
	${CMAKE_SOURCE_DIR}/include
	${CMAKE_SOURCE_DIR}/deps/GLAD/include
	${CMAKE_SOURCE_DIR}/deps/GLFW/include
	${CMAKE_SOURCE_DIR}/deps)

target_link_libraries(pgl PUBLIC
	${CMAKE_SOURCE_DIR}/deps/GLFW/lib-vc2022/glfw3.lib)

add_executable(mandelbulb application.cc)

target_link_libraries(mandelbulb PUBLIC
	pgl)

add_custom_command(TARGET mandelbulb PRE_BUILD 
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_SOURCE_DIR}/res
                ${CMAKE_CURRENT_BINARY_DIR}/res)