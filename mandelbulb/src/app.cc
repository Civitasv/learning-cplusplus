#include "app.h"

#include <iostream>

#include "error.h"

App::App()
    : window("Game Of Life", 1280, 640),
      mandelbrot(window, 1.0f),
      is_mouse_button_down(false),
      prev_x(-1),
      prev_y(-1),
      current_x(-1),
      current_y(-1) {
  SetTargetFPS(60);  // Set our game to run at 60 frames-per-second

  // Enable Multi Sampling Anti Aliasing 4x (if available)
  SetConfigFlags(FLAG_MSAA_4X_HINT);
}

App::~App() {}

void App::OnEvent() {
  if (GetMouseWheelMove() != 0) {
    mandelbrot.AddZoom(GetMouseWheelMove());
  }

  if (is_mouse_button_down) {
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
      current_x = GetMouseX();
      current_y = GetMouseY();
      return;
    }
  }

  if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
    is_mouse_button_down = true;
    prev_x = GetMouseX();
    prev_y = GetMouseY();
  } else if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
    is_mouse_button_down = false;
    mandelbrot.SetRect(prev_x, prev_y, current_x, current_y);
    prev_x = -1;
    prev_y = -1;
    current_x = -1;
    current_y = -1;
  }
}

void App::OnRender() {
  window.Begin();
  ClearBackground({100, 100, 100, 255});

  // Render here
  mandelbrot.Render();

  if (prev_x != -1 && prev_y != -1 && current_x != -1 && current_y != -1) {
    if (prev_x < current_x) {
      if (prev_y < current_y) {
        DrawRectangleLines(prev_x, prev_y, abs(current_x - prev_x),
                           abs(current_y - prev_y), ORANGE);
      } else {
        DrawRectangleLines(prev_x, current_y, abs(current_x - prev_x),
                           abs(current_y - prev_y), ORANGE);
      }
    } else {
      if (prev_y > current_y) {
        DrawRectangleLines(current_x, current_y, abs(current_x - prev_x),
                           abs(current_y - prev_y), ORANGE);
      } else {
        DrawRectangleLines(current_x, prev_y, abs(current_x - prev_x),
                           abs(current_y - prev_y), ORANGE);
      }
    }
  }
  window.End();
}

int App::Run() {
  // Custom file dialog

  while (!WindowShouldClose()) {
    OnEvent();

    OnRender();
  }

  return 0;
}