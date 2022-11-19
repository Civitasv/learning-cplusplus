#include "app.h"

#include <iostream>

#include "error.h"
#include "raygui.h"

App::App()
    : window("Game Of Life", 1280, 640),
      game_of_life(1000, 1000, 20),
      is_mouse_button_down(false),
      prev_x(-1),
      prev_y(-1) {
  SetTargetFPS(60);  // Set our game to run at 60 frames-per-second

  // Enable Multi Sampling Anti Aliasing 4x (if available)
  SetConfigFlags(FLAG_MSAA_4X_HINT);
  SetConfigFlags(FLAG_WINDOW_HIGHDPI);
}

App::~App() {}

void App::OnEvent() {
  /*if (IsKeyDown(KEY_RIGHT)) game_of_life.AddCols(10);
  if (IsKeyDown(KEY_LEFT)) game_of_life.AddCols(-10);
  if (IsKeyDown(KEY_UP)) game_of_life.AddRows(-10);
  if (IsKeyDown(KEY_DOWN)) game_of_life.AddRows(10);*/
  if (IsKeyDown(KEY_ENTER)) {
    // game start
    game_of_life.Start(true);
  }

  if (IsKeyDown(KEY_SPACE)) {
    // game pause
    game_of_life.Start(false);
  }

  if (IsKeyDown(KEY_Q)) {
    // game reset
    game_of_life.Start(false);
    game_of_life.Init();
  }

  game_of_life.Next();

  if (GetMouseWheelMove() != 0) {
    game_of_life.AddSize(GetMouseWheelMove());
  }

  if (is_mouse_button_down) {
    int x = GetMouseX();
    int y = GetMouseY();
    game_of_life.MoveStartX(x - prev_x);
    game_of_life.MoveStartY(y - prev_y);
  }

  if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
    game_of_life.ToggleState(GetMouseX(), GetMouseY());
  }

  if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
    is_mouse_button_down = true;
    prev_x = GetMouseX();
    prev_y = GetMouseY();
  } else {
    is_mouse_button_down = false;
    prev_x = -1;
    prev_y = -1;
  }

  if (IsFileDropped()) {
    int count = 0;
    FilePathList list = LoadDroppedFiles();
    char** files = list.paths;
    if (IsFileExtension(files[0], ".rle")) {
      char* data = LoadFileText(files[0]);

      game_of_life.ParseRLEFormat(data, window.GetW(), window.GetH());

      UnloadFileText(data);
    }
    UnloadDroppedFiles(list);
  }
}

void App::OnRender() {
  window.Begin();
  ClearBackground({100, 100, 100, 255});
  // Render here
  game_of_life.Render(window);

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