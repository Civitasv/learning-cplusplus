#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class RenderWindow {
 public:
  RenderWindow(const char *p_title, int p_w, int p_h);

  SDL_Texture *LoadTexture(const char *p_filepath);

  /// Clear Texture
  void Clear();
  /// Render Texture
  void Render(SDL_Texture *p_tex);
  /// Display Texture
  void Display();

  // CleanUp window, you cannot leak my memory!
  void CleanUp();

 private:
  SDL_Window *window;
  SDL_Renderer *render;
};