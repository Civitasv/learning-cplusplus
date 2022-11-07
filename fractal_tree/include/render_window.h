#pragma once

#include <SDL.h>
#include <SDL_image.h>

class RenderWindow {
 public:
  RenderWindow() {}
  RenderWindow(const char *title, int w, int h);

  ~RenderWindow();

  SDL_Texture *LoadTexture(const char *filepath);

  /// Clear
  void Clear();

  /// Display
  void Display();

 private:
  SDL_Window *window_;
  SDL_Renderer *render_;
};