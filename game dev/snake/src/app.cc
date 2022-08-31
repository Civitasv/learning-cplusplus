#include "include/app.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>
#include <memory>

#include "include/sdl_handler.h"
#include "include/snake.h"

App::App()
    : running(true), pause(false), cols(30), rows(15), size(20), score(0) {}

bool App::OnInitialize() {
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    std::cout << "HEY.. SDL_Init FAILED: " << SDL_GetError() << std::endl;
    return false;
  }
  if (!(IMG_Init(IMG_INIT_PNG))) {
    std::cout << "HEY.. IMG_Init FAILED: " << SDL_GetError() << std::endl;
    return false;
  }

  handler = {"Snake", 1280, 720, 300, 200};

  header_tex = handler.LoadTexture("res/gfx/header.png");
  tails_tex = handler.LoadTexture("res/gfx/tails.png");
  food_tex = handler.LoadTexture("res/gfx/food.png");
  brick_tex = handler.LoadTexture("res/gfx/brick.png");
  score_tex = handler.LoadTexture("res/gfx/score.png");
  numbers = {handler.LoadTexture("res/gfx/zero.png"),
             handler.LoadTexture("res/gfx/one.png"),
             handler.LoadTexture("res/gfx/two.png"),
             handler.LoadTexture("res/gfx/three.png"),
             handler.LoadTexture("res/gfx/four.png"),
             handler.LoadTexture("res/gfx/five.png"),
             handler.LoadTexture("res/gfx/six.png"),
             handler.LoadTexture("res/gfx/seven.png"),
             handler.LoadTexture("res/gfx/eight.png"),
             handler.LoadTexture("res/gfx/nine.png")};

  snake = {7, 7, size, header_tex, tails_tex, &handler};

  food = {size, food_tex, &handler};
  food.PickLocation(rows, cols);

  // up and bottom wall
  for (int x = 0; x < cols; x++) {
    bricks.push_back({x, 0, size, brick_tex, &handler});
    bricks.push_back({x, rows - 1, size, brick_tex, &handler});
  }
  // left and right wall
  for (int y = 0; y < rows; y++) {
    bricks.push_back({0, y, size, brick_tex, &handler});
    bricks.push_back({cols - 1, y, size, brick_tex, &handler});
  }
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
  if (snake.Eat(food)) {
    food.PickLocation(rows, cols);
    score++;
  }
  snake.Update();
  if (snake.Death(rows, cols)) {
    score = 0;
  }
}

void App::OnRender() {
  if (pause) {
    return;
  }
  handler.Clear();
  DrawWall();
  DrawScore();
  food.Render();
  snake.Render();
  handler.Display();
}

void App::OnCleanUp() {
  handler.CleanUp();
  SDL_DestroyTexture(header_tex);
  SDL_DestroyTexture(tails_tex);
  SDL_DestroyTexture(brick_tex);
  SDL_DestroyTexture(food_tex);
  SDL_DestroyTexture(score_tex);
  for (SDL_Texture*& item : numbers) {
    SDL_DestroyTexture(item);
  }
  SDL_Quit();
}

void App::DrawWall() {
  for (Brick& brick : bricks) {
    brick.Render();
  }
}

void App::DrawScore() {
  handler.DrawScore(cols * (size + 2), 0, score_tex, numbers, score);
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