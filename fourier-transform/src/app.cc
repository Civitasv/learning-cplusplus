#include "app.h"

#include <iostream>

#include "error.h"
#include "fourier.h"

App::App()
    : window("Fourier Transform", 1920, 1080), fourier("svg/tag.svg", 1920, 1080) {
  SetTargetFPS(12);  // Set our game to run at 60 frames-per-second

  // Enable Multi Sampling Anti Aliasing 4x (if available)
  SetConfigFlags(FLAG_MSAA_4X_HINT);
}

App::~App() {}

void App::OnEvent() {}

void App::OnRender() {
  window.Begin();
  ClearBackground({239, 245, 245, 255});

  // Render here
  fourier.Render(window);

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