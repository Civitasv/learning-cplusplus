#include "ray_window.h"

#include "error.h"

RayWindow::RayWindow(const char* title, int w, int h) : w(w), h(h) {
  InitWindow(w, h, title);
}

RayWindow::~RayWindow() { CloseWindow(); }

void RayWindow::Begin() { BeginDrawing(); }
void RayWindow::End() { EndDrawing(); }
