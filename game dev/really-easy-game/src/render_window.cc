#include "include/render_window.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>

#include "include/entity.h"

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
  // Describe size of texture.
  SDL_Rect src;
  src.x = 0;
  src.y = 0;
  src.w = 32;
  src.h = 32;

  // Describe how we want to display the texture.
  SDL_Rect dst;
  dst.x = 0;
  dst.y = 0;
  dst.w = 32;
  dst.h = 32;

  SDL_RenderCopy(render, p_tex, &src, &dst);
}

void RenderWindow::Display() { SDL_RenderPresent(render); }

void RenderWindow::CleanUp() { SDL_DestroyWindow(window); }