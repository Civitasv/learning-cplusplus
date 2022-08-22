#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "include/brick.h"
#include "include/food.h"
#include "include/snake.h"

class RenderWindow {
 public:
  RenderWindow(const char *title, int w, int h, int left, int top);

  SDL_Texture *LoadTexture(const char *filepath);

  /// Clear
  void Clear();
  // Render snake
  void Render(Snake &snake);
  void Render(Food &food);
  void Render(Brick &brick);
  void Render(int x, int y, SDL_Texture *tex,
              std::vector<SDL_Texture *> numbers, int score);
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