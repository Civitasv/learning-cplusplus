#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <vector>

class SDL_Handler {
 public:
  SDL_Handler() {}
  SDL_Handler(const char *title, int w, int h, int left, int top);

  ~SDL_Handler();

  SDL_Texture *LoadTexture(const char *filepath);

  /// Clear
  void Clear();
  // Render rectangle
  void DrawRectangle(SDL_Rect &source, SDL_Rect &dst, SDL_Texture *tex);
  void DrawRectangle(SDL_Rect &source, SDL_Rect &dst, SDL_Texture *tex,
                     double angle);
  void DrawScore(int x, int y, SDL_Texture *tex,
                 std::vector<SDL_Texture *> numbers_tex, int score);
  /// Display
  void Display();

  int left;
  int top;

 private:
  SDL_Window *window_;
  SDL_Renderer *render_;
};