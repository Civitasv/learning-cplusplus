#pragma once

#include <SDL.h>
#include <SDL_image.h>

#include <memory>
#include <vector>

#include "include/render_window.h"

class App {
 public:
  App();
  ~App();

  void OnEvent(SDL_Event* event);
  void OnLoop();
  void OnRender();
  int Run();

 private:
  bool running;
  RenderWindow window;
  int d_angle;
};