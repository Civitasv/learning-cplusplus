#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "include/entity.h"

class RenderWindow {
 public:
  RenderWindow(const char *title, int w, int h);

  SDL_Texture *LoadTexture(const char *filepath);

  /// Clear Texture
  void Clear();
  /// Render Texture
  void Render(Entity &entity);
  /// Display Texture
  void Display();

  // CleanUp window, you cannot leak my memory!
  void CleanUp();

 private:
  SDL_Window *window_;
  SDL_Renderer *render_;
};