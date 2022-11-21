#pragma once

#include "boundary.h"
#include "ray_window.h"
#include "particle.h"

class App {
 public:
  App();
  ~App();

  void OnEvent();
  void OnRender();

  int Run();

 private:
  RayWindow window;
  Particle particle;
  std::vector<Boundary> walls;
};