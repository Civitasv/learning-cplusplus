#include "include/render_window.h"

#include <SDL.h>
#include <SDL_image.h>

#include <iostream>

#include "include/error.h"

RenderWindow::RenderWindow(const char *title, int w, int h)
    : window_(nullptr), render_(nullptr) {
  window_ = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED,
                             SDL_WINDOWPOS_UNDEFINED, w, h, SDL_WINDOW_SHOWN);
  if (window_ == nullptr) {
    Error("Window failed to init. Error:", SDL_GetError());
  }

  render_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED);
}

RenderWindow::~RenderWindow() {
  SDL_DestroyRenderer(render_);
  SDL_DestroyWindow(window_);
}

SDL_Texture *RenderWindow::LoadTexture(const char *filepath) {
  SDL_Texture *texture = nullptr;
  texture = IMG_LoadTexture(render_, filepath);

  if (texture == nullptr) {
    Error("Failed to load texture. Error:", SDL_GetError());
  }

  return texture;
}

void RenderWindow::Clear() { SDL_RenderClear(render_); }

void RenderWindow::Display() { SDL_RenderPresent(render_); }
