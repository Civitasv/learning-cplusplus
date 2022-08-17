#include "include/entity.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

Entity::Entity(double p_x, double p_y, SDL_Texture* p_tex)
    : x(p_x), y(p_y), tex(p_tex) {}