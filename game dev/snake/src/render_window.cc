#include "include/render_window.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>

RenderWindow::RenderWindow(const char *title, int w, int h, int left, int top)
    : window_(nullptr), render_(nullptr), left(left), top(top) {
  window_ = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED,
                             SDL_WINDOWPOS_UNDEFINED, w, h, SDL_WINDOW_SHOWN);
  if (window_ == nullptr) {
    std::cout << "Window failed to init. Error: " << SDL_GetError()
              << std::endl;
  }

  render_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED);
}

SDL_Texture *RenderWindow::LoadTexture(const char *filepath) {
  SDL_Texture *texture = nullptr;
  texture = IMG_LoadTexture(render_, filepath);

  if (texture == nullptr) {
    std::cout << "Failed to load texture. Error: " << SDL_GetError()
              << std::endl;
  }

  return texture;
}

void RenderWindow::Clear() { SDL_RenderClear(render_); }

void RenderWindow::Render(Snake &snake) {
  double angle = snake.x_step == -1   ? 0
                 : snake.x_step == 1  ? 180
                 : snake.y_step == 1  ? 270
                 : snake.y_step == -1 ? 90
                                      : 0;
  for (auto &item : snake.tails) {
    SDL_Rect src;
    src.x = 0;
    src.y = 0;
    src.w = 16;
    src.h = 16;

    SDL_Rect dst;
    dst.x = left + item.first * snake.size;
    dst.y = top + item.second * snake.size;
    dst.w = snake.size;
    dst.h = snake.size;
    SDL_RenderCopyEx(render_, snake.tails_tex, &src, &dst, angle, nullptr,
                     SDL_FLIP_NONE);
  }

  // render header
  SDL_Rect src;
  src.x = 0;
  src.y = 0;
  src.w = 16;
  src.h = 16;

  SDL_Rect dst;
  dst.x = left + snake.x * snake.size;
  dst.y = top + snake.y * snake.size;
  dst.w = snake.size;
  dst.h = snake.size;

  SDL_RenderCopyEx(render_, snake.header_tex, &src, &dst, angle, nullptr,
                   SDL_FLIP_NONE);
}

void RenderWindow::Render(Food &food) {
  SDL_Rect src;
  src.x = 0;
  src.y = 0;
  src.w = 16;
  src.h = 16;

  SDL_Rect dst;
  dst.x = left + food.x * food.size;
  dst.y = top + food.y * food.size;
  dst.w = food.size;
  dst.h = food.size;

  SDL_RenderCopy(render_, food.tex, &src, &dst);
}

void RenderWindow::Render(Brick &brick) {
  SDL_Rect src;
  src.x = 0;
  src.y = 0;
  src.w = 16;
  src.h = 16;

  SDL_Rect dst;
  dst.x = left + brick.x * brick.size;
  dst.y = top + brick.y * brick.size;
  dst.w = brick.size;
  dst.h = brick.size;

  SDL_RenderCopy(render_, brick.tex, &src, &dst);
}

void RenderWindow::Render(int x, int y, SDL_Texture *tex,
                          std::vector<SDL_Texture *> numbers, int val) {
  SDL_Rect src;
  src.x = 0;
  src.y = 0;
  src.w = 120;
  src.h = 32;

  SDL_Rect dst;
  dst.x = left + x;
  dst.y = top + y;
  dst.w = 120;
  dst.h = 32;

  SDL_RenderCopy(render_, tex, &src, &dst);

  src.x = 0;
  src.y = 0;
  src.w = 16;
  src.h = 16;

  dst.x = left + x + 120;
  dst.y = top + y + 8;
  dst.w = 16;
  dst.h = 16;

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

    SDL_RenderCopy(render_, numbers[mod], &src, &dst);
    dst.x -= 16;
    val /= 10;
  } while (val != 0);
}

void RenderWindow::Display() { SDL_RenderPresent(render_); }

void RenderWindow::CleanUp() {
  SDL_DestroyRenderer(render_);
  SDL_DestroyWindow(window_);
}