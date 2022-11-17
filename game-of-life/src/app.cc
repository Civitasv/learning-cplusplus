#include "app.h"

#include "game_of_life.h"
#include "error.h"
#include "raylib.h"

App::App() : window("Game Of Life", 1280, 640) {
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
  ClearBackground(RAYWHITE);
  // Render here
  Render(window);

  window.End();
}

int App::Run() {
  while (!WindowShouldClose()) {
    OnEvent();

    OnRender();
  }

  return 0;
}