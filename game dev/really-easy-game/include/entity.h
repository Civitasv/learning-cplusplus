#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class Entity {
 public:
  Entity(double p_x, double p_y, SDL_Texture* p_tex);

 private:
  double x;
  double y;
  SDL_Texture* tex;

  SDL_Rect current_frame;
};
