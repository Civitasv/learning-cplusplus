add_executable(snake run.cc sdl_handler.cc app.cc)

target_include_directories(snake PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>)

INCLUDE(FindPkgConfig)

PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
PKG_SEARCH_MODULE(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)

target_include_directories(snake PUBLIC
    ${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS})
target_link_libraries(snake ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES})