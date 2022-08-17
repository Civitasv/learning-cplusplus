#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>

#include "include/render_window.h"

int main(int argc, char* argv[]) {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cout << "HEY.. SDL_Init FAILED: " << SDL_GetError() << std::endl;
  }
  if (!(IMG_Init(IMG_INIT_PNG))) {
    std::cout << "HEY.. IMG_Init FAILED: " << SDL_GetError() << std::endl;
  }

  RenderWindow window("Game v1.0", 1280, 720);

  SDL_Texture* grass_texture = window.LoadTexture("res/gfx/ground_grass_1.png");

  bool game_running = true;
  SDL_Event event;

  // event loop
  while (game_running) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        game_running = false;
      }
      window.Clear();
      window.Render(grass_texture);
      window.Display();
    }
  }

  window.CleanUp();
  SDL_Quit();
  return 0;
}