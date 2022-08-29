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

void RenderWindow::Render(Graph &graph, int size) {
  Clear();
  for (auto &node : graph.nodes) {
    SDL_Rect src;
    src.x = left + node.p.x * size;
    src.y = top + node.p.y * size;
    src.w = size;
    src.h = size;
    if (node.status == 0) {
    } else if (node.status == 1) {
      SDL_SetRenderDrawColor(render_, 0, 0, 255, 255);
      SDL_RenderFillRect(render_, &src);
    } else if (node.status == 2) {
      SDL_SetRenderDrawColor(render_, 0, 255, 0, 255);
      SDL_RenderFillRect(render_, &src);
    } else if (node.status == 3) {
      SDL_SetRenderDrawColor(render_, 255, 0, 0, 255);
      SDL_RenderFillRect(render_, &src);
    }

    SDL_SetRenderDrawColor(render_, 255, 255, 255, 255);
    SDL_RenderDrawRect(render_, &src);
  }
  SDL_SetRenderDrawColor(render_, 0, 0, 0, 255);
  Display();
}

void RenderWindow::Display() { SDL_RenderPresent(render_); }

void RenderWindow::CleanUp() {
  SDL_DestroyRenderer(render_);
  SDL_DestroyWindow(window_);
}