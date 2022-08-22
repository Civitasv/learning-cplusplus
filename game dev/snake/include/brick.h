#pragma once

#include <SDL2/SDL.h>

class Brick {
 public:
  int x;
  int y;
  int size;

  SDL_Texture* tex;

  Brick() : x(-1), y(-1), size(0) {}
  Brick(int x, int y, int size, SDL_Texture* tex)
      : x(x), y(y), size(size), tex(tex) {}
};