#pragma once

#include "mandelbrot.h"

class App {
 public:
  App();
  ~App();

  void OnEvent();
  void OnRender();

  int Run();

 private:
  RayWindow window;
  Mandelbrot mandelbrot;
  bool is_mouse_button_down;
  int prev_x;
  int prev_y;
  int current_x;
  int current_y;
};