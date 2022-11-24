#include "ray_window.h"

#include "error.h"

RayWindow::RayWindow(const char* title, int w, int h) {
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(w, h, title);
}

RayWindow::~RayWindow() { CloseWindow(); }

void RayWindow::Begin() { BeginDrawing(); }
void RayWindow::End() { EndDrawing(); }
