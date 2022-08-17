#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class RenderWindow {
 public:
  RenderWindow(const char *p_title, int p_w, int p_h);

  SDL_Texture *LoadTexture(const char *p_filepath);

  void Clear();
  void Render(SDL_Texture *p_tex);
  void Display();

  void CleanUp();

 private:
  SDL_Window *window;
  SDL_Renderer *render;
};