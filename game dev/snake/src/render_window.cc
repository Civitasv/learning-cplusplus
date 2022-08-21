#include "include/render_window.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>

RenderWindow::RenderWindow(const char *title, int w, int h)
    : window_(nullptr), render_(nullptr) {
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
  SDL_SetRenderDrawColor(render_, 255, 255, 255, 255);
  for (auto &item : snake.tails) {
    SDL_Rect r;
    r.x = item.first * snake.size;
    r.y = item.second * snake.size;
    r.w = snake.size;
    r.h = snake.size;
    SDL_RenderFillRect(render_, &r);
  }
  SDL_Rect r;
  r.x = snake.x * snake.size;
  r.y = snake.y * snake.size;
  r.w = snake.size;
  r.h = snake.size;
  SDL_RenderFillRect(render_, &r);
  SDL_SetRenderDrawColor(render_, 0, 0, 0, 255);
}

void RenderWindow::Render(Food &food) {
  SDL_SetRenderDrawColor(render_, 255, 0, 0, 255);
  SDL_Rect r;
  r.x = food.x * food.size;
  r.y = food.y * food.size;
  r.w = food.size;
  r.h = food.size;

  SDL_RenderFillRect(render_, &r);
  SDL_SetRenderDrawColor(render_, 0, 0, 0, 255);
}

void RenderWindow::Display() { SDL_RenderPresent(render_); }

void RenderWindow::CleanUp() { SDL_DestroyWindow(window_); }