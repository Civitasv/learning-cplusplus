#include "app.h"

#include "clock.h"
#include "error.h"
#include "raylib.h"

App::App() : window("Just A Clock", 1280, 640) {
  SetTargetFPS(60);  // Set our game to run at 60 frames-per-second

  // Enable Multi Sampling Anti Aliasing 4x (if available)
  SetConfigFlags(FLAG_MSAA_4X_HINT);
}

App::~App() {}

void App::OnEvent() {
  if (IsKeyDown(KEY_RIGHT))
    ;
  if (IsKeyDown(KEY_LEFT))
    ;
  if (IsKeyDown(KEY_UP))
    ;
  if (IsKeyDown(KEY_DOWN))
    ;
}

void App::OnRender() {
  window.Begin();
  ClearBackground(WHITE);
  // Render here
  RenderClock(window);

  window.End();
}

int App::Run() {
  while (!WindowShouldClose()) {
    OnEvent();

    OnRender();
  }

  return 0;
}