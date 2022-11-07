#include "include/app.h"

#include <SDL.h>
#include <SDL_image.h>

#include <iostream>
#include <memory>
#include <random>

#include "include/error.h"
#include "include/render_window.h"

App::App() : running(true), window("Fractal Tree", 1280, 640) {
  // Init Everything
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    Error("SDL_Init FAILED:", SDL_GetError());
    return;
  }
  if (!(IMG_Init(IMG_INIT_PNG))) {
    Error("IMG_Init FAILED:", SDL_GetError());
    return;
  }
}

App::~App() { SDL_Quit(); }

void App::OnEvent(SDL_Event* event) {
  if (event->type == SDL_QUIT) {
    running = false;
  } else if (event->type == SDL_KEYDOWN) {
    SDL_Keycode code = event->key.keysym.sym;
    if (code == SDLK_d) {
    }
  }
}

void App::OnLoop() {}

void App::OnRender() {
  window.Clear();
  // Render Here
  window.Display();
}

int App::Run() {
  SDL_Event event;

  while (running) {
    while (SDL_PollEvent(&event)) {
      // 1. Event
      OnEvent(&event);
    }
    // 2. Loop, update data according to event
    OnLoop();
    // 3. Render
    OnRender();

    SDL_Delay(200);
  }

  return 0;
}