#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <memory>
#include <vector>

#include "include/brick.h"
#include "include/food.h"
#include "include/render_window.h"
#include "include/snake.h"

class App {
 public:
  App();

  bool OnInitialize();
  void OnEvent(SDL_Event* event);
  void OnLoop();
  void OnRender();
  void OnCleanUp();
  int Run();

 private:
  void PickFoodLocation();
  void DrawWall();
  void DrawScore();
  bool running;
  bool pause;
  RenderWindow window;
  Snake snake;
  Food food;
  std::vector<Brick> bricks;

  SDL_Texture* header_tex;
  SDL_Texture* tails_tex;
  SDL_Texture* brick_tex;
  SDL_Texture* food_tex;
  SDL_Texture* score_tex;
  std::vector<SDL_Texture *> numbers;

  int cols;
  int rows;
  int size;

  int score;
};