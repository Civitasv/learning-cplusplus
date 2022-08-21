#include "include/app.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>
#include <memory>
#include <random>

#include "include/render_window.h"
#include "include/snake.h"

App::App()
    : running(true),
      cols(30),
      rows(15),
      size(20),
      window({"Snake", 1280, 720}) {
  snake = {7, 7, size};
  food.size = size;
  PickFoodLocation();
}

void App::PickFoodLocation() {
  std::random_device dev;
  std::mt19937 rng(dev());
  std::uniform_int_distribution<std::mt19937::result_type> a(0, rows);
  std::uniform_int_distribution<std::mt19937::result_type> b(0, cols);

  food.y = a(rng);
  food.x = b(rng);
}

bool App::OnInitialize() {
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    std::cout << "HEY.. SDL_Init FAILED: " << SDL_GetError() << std::endl;
    return false;
  }
  if (!(IMG_Init(IMG_INIT_PNG))) {
    std::cout << "HEY.. IMG_Init FAILED: " << SDL_GetError() << std::endl;
    return false;
  }

  return true;
}

void App::OnEvent(SDL_Event* event) {
  if (event->type == SDL_QUIT) {
    running = false;
  } else if (event->type == SDL_KEYDOWN) {
    SDL_Keycode code = event->key.keysym.sym;
    if (code == SDLK_LEFT) {
      snake.Dir(-1, 0);
    }  // left arrow
    else if (code == SDLK_RIGHT) {
      snake.Dir(1, 0);
    }  // right arrow
    else if (code == SDLK_UP) {
      snake.Dir(0, -1);
    }  // up arrow
    else if (code == SDLK_DOWN) {
      snake.Dir(0, 1);
    }  // down arrow
  }
}

void App::OnLoop() {
  snake.Update();
  snake.Death(rows, cols);

  if (snake.Eat(food)) {
    PickFoodLocation();
  }
}

void App::OnRender() {
  window.Clear();
  window.Render(snake);
  window.Render(food);
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

    SDL_Delay(100);
  }

  // 5. Cleanup
  OnCleanUp();
  return 0;
}