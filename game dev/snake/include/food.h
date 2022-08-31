#pragma once

#include <SDL2/SDL.h>

#include <random>

#include "include/sdl_handler.h"

class Food {
 public:
  int x;
  int y;
  int size;

  SDL_Texture* tex;
  SDL_Handler* handler;

  Food() : x(-1), y(-1), size(0) {}
  Food(int size, SDL_Texture* tex, SDL_Handler* handler)
      : size(size), tex(tex), handler(handler) {}

  void Render() {
    SDL_Rect src{0, 0, 16, 16};

    SDL_Rect dst{handler->left + x * size, handler->top + y * size, size, size};

    handler->DrawRectangle(src, dst, tex);
  }

  void PickLocation(int rows, int cols) {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> a(1, rows - 2);
    std::uniform_int_distribution<std::mt19937::result_type> b(1, cols - 2);

    y = a(rng);
    x = b(rng);
  }
};