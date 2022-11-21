#include "app.h"

#include <iostream>

#include "error.h"

App::App() : window("Ray Casting", 1920, 1000), particle({50, 100}) {
  SetTargetFPS(60);  // Set our game to run at 60 frames-per-second

  // Enable Multi Sampling Anti Aliasing 4x (if available)
  SetConfigFlags(FLAG_MSAA_4X_HINT);

  // borders
  walls.push_back({{0, 0}, {window.GetW() * 1.0f, 0}, true});
  walls.push_back({{0, 0}, {0, window.GetH() * 1.0f}, true});
  walls.push_back({{0, window.GetH() * 1.0f},
                   {window.GetW() * 1.0f, window.GetH() * 1.0f},
                   true});
  walls.push_back({{window.GetW() * 1.0f, 0},
                   {window.GetW() * 1.0f, window.GetH() * 1.0f},
                   true});
  walls.push_back({{200, 200}, {400, 300}});
  walls.push_back({{700, 600}, {800, 500}});
  walls.push_back({{800, 200}, {900, 400}});
  walls.push_back({{320, 470}, {450, 570}});
}

App::~App() {}

void App::OnEvent() {
  particle.Update({GetMouseX() * 1.0f, GetMouseY() * 1.0f}, walls);
}

void App::OnRender() {
  window.Begin();
  ClearBackground(BLACK);
  for (auto& wall : walls) {
    wall.Render();
  }
  particle.Look(walls);
  window.End();
}

int App::Run() {
  while (!WindowShouldClose()) {
    OnEvent();

    OnRender();
  }

  return 0;
}