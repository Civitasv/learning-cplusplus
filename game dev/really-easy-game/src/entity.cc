#include "include/entity.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

Entity::Entity(float x, float y, SDL_Texture* tex) : x_(x), y_(y), tex_(tex) {
  current_frame_.x = 0;
  current_frame_.y = 0;
  current_frame_.w = 32;
  current_frame_.h = 32;
}

float Entity::x() { return x_; }
float Entity::y() { return y_; }

SDL_Texture* Entity::tex() { return tex_; }

SDL_Rect Entity::current_frame() { return current_frame_; }