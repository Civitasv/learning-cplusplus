#pragma once

#include <vector>

#include "raylib.h"

class RayWindow {
 public:
  RayWindow(const char* title, int w, int h);
  ~RayWindow();

  void Begin();

  void End();

  int GetW() { return GetScreenWidth(); }
  int GetH() { return GetScreenHeight(); }
};