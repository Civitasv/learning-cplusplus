#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <memory>
#include <vector>

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
  bool running;
  RenderWindow window;
  Snake snake;
  Food food;
  int cols;
  int rows;
  int size;
};