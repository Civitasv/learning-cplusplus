#include "include/render_window.h"

#include <SDL.h>
#include <SDL_image.h>

#include <iostream>

#include "include/error.h"

RenderWindow::RenderWindow(const char *title, int w, int h)
    : window(nullptr), render(nullptr), w(w), h(h) {
  window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED,
                            SDL_WINDOWPOS_UNDEFINED, w, h, SDL_WINDOW_SHOWN);
  if (window == nullptr) {
    Error("Window failed to init. Error:", SDL_GetError());
  }

  render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

RenderWindow::~RenderWindow() {
  SDL_DestroyRenderer(render);
  SDL_DestroyWindow(window);
}

void RenderWindow::RenderLine(int x1, int y1, int x2, int y2) {
  SDL_SetRenderDrawColor(render, 255, 255, 255, 255);
  SDL_RenderDrawLine(render, x1, y1, x2, y2);

  // 背景色
  SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
}

SDL_Texture *RenderWindow::LoadTexture(const char *filepath) {
  SDL_Texture *texture = nullptr;
  texture = IMG_LoadTexture(render, filepath);

  if (texture == nullptr) {
    Error("Failed to load texture. Error:", SDL_GetError());
  }

  return texture;
}

void RenderWindow::Clear() { SDL_RenderClear(render); }

void RenderWindow::Display() { SDL_RenderPresent(render); }
