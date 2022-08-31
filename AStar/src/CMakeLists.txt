add_executable(Astar run.cc render_window.cc app.cc graph.cc)

target_include_directories(Astar PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>)

INCLUDE(FindPkgConfig)

PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
PKG_SEARCH_MODULE(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)

target_include_directories(Astar PUBLIC
    ${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS})
target_link_libraries(Astar ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES})