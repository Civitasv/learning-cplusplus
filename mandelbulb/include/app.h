#pragma once

#include "mandelbulb.h"

class App {
 public:
  App();
  ~App();

  void OnEvent();
  void OnRender();

  int Run();

 private:
  Camera camera;
  RayWindow window;
  Mandelbulb mandelbulb;
};