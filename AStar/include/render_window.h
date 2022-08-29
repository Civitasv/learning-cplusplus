#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "include/graph.h"

class RenderWindow {
 public:
  RenderWindow() {}
  RenderWindow(const char *title, int w, int h, int left, int top);

  SDL_Texture *LoadTexture(const char *filepath);

  /// Clear
  void Clear();

  // Render
  void Render(Graph& graph, int size);

  /// Display
  void Display();

  // CleanUp window, you cannot leak my memory!
  void CleanUp();

 private:
  SDL_Window *window_;
  SDL_Renderer *render_;
  int left;
  int top;
};