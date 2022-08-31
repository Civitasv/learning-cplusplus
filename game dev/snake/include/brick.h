#pragma once

#include <SDL2/SDL.h>

#include "include/sdl_handler.h"

class Brick {
 public:
  int x;
  int y;
  int size;

  SDL_Texture* tex;
  SDL_Handler* handler;

  Brick() : x(-1), y(-1), size(0) {}
  Brick(int x, int y, int size, SDL_Texture* tex, SDL_Handler* handler)
      : x(x), y(y), size(size), tex(tex), handler(handler) {}

  void Render() {
    SDL_Rect src{0, 0, 16, 16};

    SDL_Rect dst{handler->left + x * size, handler->top + y * size, size, size};

    handler->DrawRectangle(src, dst, tex);
  }
};