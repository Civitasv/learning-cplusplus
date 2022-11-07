add_executable(fractal_tree run.cc render_window.cc app.cc)

target_include_directories(fractal_tree PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>)

set(SDL2_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/deps/sdl2/include)
set(SDL2IMAGE_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/deps/sdl2_image/include)

set(SDL2_X64_LIB_PATH ${CMAKE_SOURCE_DIR}/deps/sdl2/lib/x64)
set(SDL2_IMAGE_X64_LIB_PATH ${CMAKE_SOURCE_DIR}/deps/sdl2_image/lib/x64)

set(SDL2_LIBRARIES ${SDL2_X64_LIB_PATH}/SDL2.lib ${SDL2_X64_LIB_PATH}/SDL2main.lib)
set(SDL2IMAGE_LIBRARIES ${SDL2_IMAGE_X64_LIB_PATH}/SDL2_image.lib)

target_include_directories(fractal_tree PUBLIC
    ${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS})
target_link_libraries(fractal_tree ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES})