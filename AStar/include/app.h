#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <memory>
#include <vector>

#include "include/render_window.h"

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
  bool running;
  bool pause;
  RenderWindow window;
  
  int size;
};