#pragma once

#include <SDL.h>
#include <SDL_image.h>

class RenderWindow {
 public:
  RenderWindow() : window(nullptr), render(nullptr) {}
  RenderWindow(const char *title, int w, int h);

  ~RenderWindow();

  void RenderLine(int x1, int y1, int x2, int y2);

  SDL_Texture *LoadTexture(const char *filepath);

  /// Clear
  void Clear();

  /// Display
  void Display();

  int GetW() { return w; }
  int GetH() { return h; }

 private:
  SDL_Window *window;
  SDL_Renderer *render;
  int w;
  int h;
};