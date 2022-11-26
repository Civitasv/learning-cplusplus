#pragma once

#include "fourier.h"
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
  Fourier fourier;
};