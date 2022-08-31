#include "include/sdl_handler.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>

SDL_Handler::SDL_Handler(const char *title, int w, int h, int left, int top)
    : window_(nullptr), render_(nullptr), left(left), top(top) {
  window_ = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED,
                             SDL_WINDOWPOS_UNDEFINED, w, h, SDL_WINDOW_SHOWN);
  if (window_ == nullptr) {
    std::cout << "Window failed to init. Error: " << SDL_GetError()
              << std::endl;
  }

  render_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED);
}

SDL_Texture *SDL_Handler::LoadTexture(const char *filepath) {
  SDL_Texture *texture = nullptr;
  texture = IMG_LoadTexture(render_, filepath);

  if (texture == nullptr) {
    std::cout << "Failed to load texture. Error: " << SDL_GetError()
              << std::endl;
  }

  return texture;
}

void SDL_Handler::DrawScore(int x, int y, SDL_Texture *tex,
                            std::vector<SDL_Texture *> numbers_tex, int val) {
  // draw text "score"
  SDL_Rect src = {0, 0, 120, 32};

  SDL_Rect dst = {left + x, top + y, 120, 32};

  DrawRectangle(src, dst, tex);

  // draw score number
  src = {0, 0, 16, 16};
  dst = {left + x + 120, top + y + 8, 16, 16};

  int count = 0;
  int temp = val;
  while (temp != 0) {
    count += 1;
    temp /= 10;
  }
  if (count > 0) {
    dst.x += 16 * (count - 1);
  }

  do {
    int mod = val % 10;

    DrawRectangle(src, dst, numbers_tex[mod]);
    dst.x -= 16;
    val /= 10;
  } while (val != 0);
}

void SDL_Handler::DrawRectangle(SDL_Rect &source, SDL_Rect &dst,
                                SDL_Texture *tex) {
  if (tex != nullptr) {
    SDL_RenderCopy(render_, tex, &source, &dst);
  } else {
    std::cout << "DrawRectangle: text was nullptr" << std::endl;
  }
}

void SDL_Handler::DrawRectangle(SDL_Rect &source, SDL_Rect &dst,
                                SDL_Texture *tex, double angle) {
  if (tex != nullptr) {
    SDL_RenderCopyEx(render_, tex, &source, &dst, angle, nullptr,
                     SDL_FLIP_NONE);
  } else {
    std::cout << "DrawRectangle: text was nullptr" << std::endl;
  }
}

void SDL_Handler::Clear() { SDL_RenderClear(render_); }

void SDL_Handler::Display() { SDL_RenderPresent(render_); }

void SDL_Handler::CleanUp() {
  SDL_DestroyRenderer(render_);
  SDL_DestroyWindow(window_);
}