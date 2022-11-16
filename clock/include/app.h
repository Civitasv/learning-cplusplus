#pragma once

#include "ray_window.h"

class App {
 public:
  App();
  ~App();

  void OnEvent();
  void OnRender();

  int Run();

 private:
  RayWindow window;
};