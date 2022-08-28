#include "include/app.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>
#include <memory>
#include <random>

#include "include/render_window.h"

App::App() : running(true), pause(false), size(20) {}

bool App::OnInitialize() {
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    std::cout << "HEY.. SDL_Init FAILED: " << SDL_GetError() << std::endl;
    return false;
  }
  if (!(IMG_Init(IMG_INIT_PNG))) {
    std::cout << "HEY.. IMG_Init FAILED: " << SDL_GetError() << std::endl;
    return false;
  }

  window = {"A* Algrorithm", 1280, 720, 300, 200};

  return true;
}

void App::OnEvent(SDL_Event* event) {
  if (pause && event->key.keysym.sym != SDLK_r) {
    return;
  }
  if (event->type == SDL_QUIT) {
    running = false;
  } else if (event->type == SDL_KEYDOWN) {
    SDL_Keycode code = event->key.keysym.sym;
    if (code == SDLK_LEFT) {
    }  // left arrow
    else if (code == SDLK_RIGHT) {
    }  // right arrow
    else if (code == SDLK_UP) {
    }  // up arrow
    else if (code == SDLK_DOWN) {
    }  // down arrow
    else if (code == SDLK_q) {
      running = false;
    }  // q for quit
    else if (code == SDLK_r) {
      pause = !pause;
    }  // r for pause and resume
  }
}

void App::OnLoop() {
  if (pause) {
    return;
  }
}

void App::OnRender() {
  if (pause) {
    return;
  }
  window.Clear();
  window.Render();
  window.Display();
}

void App::OnCleanUp() {
  window.CleanUp();
  SDL_Quit();
}

int App::Run() {
  // 1. Initialize
  if (!OnInitialize()) {
    return -1;
  }

  SDL_Event event;

  while (running) {
    while (SDL_PollEvent(&event)) {
      // 2. Event
      OnEvent(&event);
    }
    // 3. Loop, update data according to event
    OnLoop();
    // 4. Render
    OnRender();

    SDL_Delay(200);
  }

  // 5. Cleanup
  OnCleanUp();
  return 0;
}