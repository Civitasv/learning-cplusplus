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

void RenderWindow::Render() {}

void RenderWindow::Display() { SDL_RenderPresent(render_); }

void RenderWindow::CleanUp() {
  SDL_DestroyRenderer(render_);
  SDL_DestroyWindow(window_);
}