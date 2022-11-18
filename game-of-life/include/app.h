#pragma once

#include "ray_window.h"
#include "game_of_life.h"

class App {
 public:
  App();
  ~App();

  void OnEvent();
  void OnRender();

  int Run();

 private:
  RayWindow window;
  GameOfLife game_of_life;
  bool is_mouse_button_down;
  int prev_x;
  int prev_y;
};