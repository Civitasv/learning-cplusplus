#pragma once

#include <vector>

#include "raylib.h"

class RayWindow {
 public:
  RayWindow(const char* title, int w, int h);
  ~RayWindow();

  void Begin();

  void End();

  int GetH() { return h; }
  int GetW() { return w; }

 private:
  int w;
  int h;
};