#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class Entity {
 public:
  Entity(float x, float y, SDL_Texture* tex);

  float x();
  float y();

  SDL_Texture* tex();
  SDL_Rect current_frame();

 private:
  // Coordinate x that texture will be displayed.
  float x_;

  // Coordinate y that texture will be displayed.
  float y_;

  // Displayed texture.
  SDL_Texture* tex_;

  // Pos and Size of current texture.
  SDL_Rect current_frame_;
};
