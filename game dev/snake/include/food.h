#pragma once

#include <SDL2/SDL.h>

class Food {
 public:
  int x;
  int y;
  int size;

  SDL_Texture* tex;

  Food() : x(-1), y(-1), size(0) {}
  Food(int x, int y, int size) : x(x), y(y), size(size) {}
};