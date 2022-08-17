#include "include/render_window.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>

#include "include/entity.h"

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
void RenderWindow::Render(Entity &entity) {
  // Describe size of texture.
  SDL_Rect src;
  src.x = entity.current_frame().x;
  src.y = entity.current_frame().y;
  src.w = entity.current_frame().w;
  src.h = entity.current_frame().h;

  // Describe how and where we want to display the texture.
  SDL_Rect dst;
  int scale = 2;
  dst.x = entity.x() * scale;
  dst.y = entity.y() * scale;
  dst.w = entity.current_frame().w * scale;
  dst.h = entity.current_frame().h * scale;

  SDL_RenderCopy(render_, entity.tex(), &src, &dst);
}

void RenderWindow::Display() { SDL_RenderPresent(render_); }

void RenderWindow::CleanUp() { SDL_DestroyWindow(window_); }