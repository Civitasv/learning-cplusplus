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
      pause(false),
      cols(30),
      rows(15),
      size(20),
      score(0),
      window({"Snake", 1280, 720, 300, 200}) {
  header_tex = window.LoadTexture("res/gfx/header.png");
  tails_tex = window.LoadTexture("res/gfx/tails.png");
  food_tex = window.LoadTexture("res/gfx/food.png");
  brick_tex = window.LoadTexture("res/gfx/brick.png");
  score_tex = window.LoadTexture("res/gfx/score.png");
  numbers = {window.LoadTexture("res/gfx/zero.png"),
             window.LoadTexture("res/gfx/one.png"),
             window.LoadTexture("res/gfx/two.png"),
             window.LoadTexture("res/gfx/three.png"),
             window.LoadTexture("res/gfx/four.png"),
             window.LoadTexture("res/gfx/five.png"),
             window.LoadTexture("res/gfx/six.png"),
             window.LoadTexture("res/gfx/seven.png"),
             window.LoadTexture("res/gfx/eight.png"),
             window.LoadTexture("res/gfx/nine.png")};

  snake = {7, 7, size};
  snake.header_tex = header_tex;
  snake.tails_tex = tails_tex;

  food.size = size;
  food.tex = food_tex;
  PickFoodLocation();

  // up and bottom wall
  for (int x = 0; x < cols; x++) {
    bricks.push_back({x, 0, size, brick_tex});
    bricks.push_back({x, rows - 1, size, brick_tex});
  }
  // left and right wall
  for (int y = 0; y < rows; y++) {
    bricks.push_back({0, y, size, brick_tex});
    bricks.push_back({cols - 1, y, size, brick_tex});
  }
}

void App::PickFoodLocation() {
  std::random_device dev;
  std::mt19937 rng(dev());
  std::uniform_int_distribution<std::mt19937::result_type> a(1, rows - 2);
  std::uniform_int_distribution<std::mt19937::result_type> b(1, cols - 2);

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
    PickFoodLocation();
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
  window.Clear();
  DrawWall();
  DrawScore();
  window.Render(food);
  window.Render(snake);
  window.Display();
}

void App::OnCleanUp() {
  window.CleanUp();
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
    window.Render(brick);
  }
}

void App::DrawScore() {
  window.Render(cols * (size + 2), 0, score_tex, numbers, score);
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