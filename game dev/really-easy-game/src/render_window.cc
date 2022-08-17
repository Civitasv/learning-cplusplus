#include "include/render_window.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>

RenderWindow::RenderWindow(const char *p_title, int p_w, int p_h)
    : window(nullptr), render(nullptr) {
  window =
      SDL_CreateWindow(p_title, SDL_WINDOWPOS_UNDEFINED,
                       SDL_WINDOWPOS_UNDEFINED, p_w, p_h, SDL_WINDOW_SHOWN);
  if (window == nullptr) {
    std::cout << "Window failed to init. Error: " << SDL_GetError()
              << std::endl;
  }

  render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

SDL_Texture *RenderWindow::LoadTexture(const char *p_filepath) {
  SDL_Texture *texture = nullptr;
  texture = IMG_LoadTexture(render, p_filepath);

  if (texture == nullptr) {
    std::cout << "Failed to load texture. Error: " << SDL_GetError()
              << std::endl;
  }

  return texture;
}

void RenderWindow::Clear() { SDL_RenderClear(render); }
void RenderWindow::Render(SDL_Texture *p_tex) {
  SDL_RenderCopy(render, p_tex, nullptr, nullptr);
}

void RenderWindow::Display() { SDL_RenderPresent(render); }

void RenderWindow::CleanUp() { SDL_DestroyWindow(window); }