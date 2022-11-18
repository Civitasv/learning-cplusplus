#pragma once
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include "ray_window.h"

struct Grid {
  int row;
  int col;
  int state;
};

class GameOfLife {
 public:
  GameOfLife(int rows, int cols, int size)
      : rows(rows),
        cols(cols),
        size(size),
        padding(10),
        width_ratio(0.7),
        height_ratio(1.0),
        cycle(0),
        thickness(1.0f),
        move_x(0.0f),
        move_y(0.0f),
        all_dead(false),
        start(false) {
    Init();
  }

  void Init() {
    grids.clear();
    for (int i = 0; i < rows; i++) {
      std::vector<Grid> grid_row;
      for (int j = 0; j < cols; j++) {
        grid_row.push_back({i, j, 0});
      }
      grids.push_back(grid_row);
    }
    cycle = 0;
    all_dead = false;
    thickness = 1.0f;
    move_x = 0.0f;
    move_y = 0.0f;
  }

  void Render(RayWindow& window) {
    int can_rows = window.GetH() / size;
    int can_cols = window.GetW() / size;
    int start_rows = rows > can_rows ? (rows - can_rows) / 2 : 0;
    int start_cols = cols > can_cols ? (cols - can_cols) / 2 : 0;
    int start_x = start_rows * size;
    int start_y = start_cols * size;
    render_x = start_x + move_x;
    render_y = start_y + move_y;
    {
      // Render rule
      DrawText("Press Enter to Start..", window.GetW() * 0.7 + padding,
               window.GetH() * 0.1, 20, RAYWHITE);
      DrawText("Press Space to Pause..", window.GetW() * 0.7 + padding,
               window.GetH() * 0.2, 20, RAYWHITE);
      DrawText("Press q to Reset..", window.GetW() * 0.7 + padding,
               window.GetH() * 0.3, 20, RAYWHITE);

      DrawText("Use Mouse Wheel to Zoom In and Out..",
               window.GetW() * 0.7 + padding, window.GetH() * 0.4, 20,
               RAYWHITE);

      DrawText("Use Mouse Left Button to Drag..", window.GetW() * 0.7 + padding,
               window.GetH() * 0.5, 20, RAYWHITE);
      DrawText("Use Mouse Right Button to Toggle State..",
               window.GetW() * 0.7 + padding, window.GetH() * 0.6, 20,
               RAYWHITE);
    }
    {
      // Render game state
      std::string text = "Game is: ";
      std::string state_str = start ? "RUNNING" : all_dead ? "END" : "PAUSE";
      DrawText((text + state_str).c_str(), window.GetW() * 0.7 + padding,
               window.GetH() * 0.7, 20, RAYWHITE);
    }
    {
      // Render current cycle
      std::string text = "Current generation: ";
      std::string cycle_str = all_dead ? "ALL DEAD" : std::to_string(cycle);
      DrawText((text + cycle_str).c_str(), window.GetW() * 0.7 + padding,
               window.GetH() * 0.8, 20, RED);
    }
    {
      // Render background
      DrawRectangle(0, 0, window.GetW() * width_ratio,
                    window.GetH() * height_ratio, {64, 64, 64, 255});
    }
    {
      // Render All Grids
      Color state_1 = {204, 204, 204, 255};
      Color state_2 = ORANGE;
      float width = window.GetW() * width_ratio;
      float height = window.GetH() * height_ratio;

      for (auto& grid_row : grids) {
        for (auto& item : grid_row) {
          int x = item.col * size - render_x;
          int y = item.row * size - render_y;
          if (!(x >= padding && (x + size) <= width - padding && y >= padding &&
                (y + size) <= height - padding)) {
            continue;
          }

          DrawRectangleLinesEx({x * 1.0f, y * 1.0f, size, size}, thickness,
                               state_1);
          if (item.state == 1)
            DrawRectangle(x + 1, y + 1, size - 2, size - 2, state_2);
        }
      }
    }
  }

  void Next() {
    if (all_dead || !start) {
      return;
    }
    cycle += 1;
    all_dead = true;

    std::vector<std::vector<Grid>> next = grids;
    for (int i = 0; i < rows; i++) {
      for (int j = 0; j < cols; j++) {
        int sum = 0;
        for (int a = -1; a < 2; a++) {
          for (int b = -1; b < 2; b++) {
            if (a == 0 && b == 0) continue;
            if (i + a < 0 || i + a >= rows) continue;
            if (j + b < 0 || j + b >= cols) continue;
            /*std::cout << grids.size() << ": " << grids[0].size()
                      << "row: " << i + a << " col: " << j + b << '\n';*/
            sum += grids[i + a][j + b].state;
          }
        }
        int state = grids[i][j].state;
        // Each cell with one or no neighbors dies, as if by solitude.
        if (state == 1 && (sum == 1 || sum == 0)) {
          next[i][j].state = 0;
          all_dead = false;
        }
        // Each cell with four or more neighbors dies, as if by overpopulation.
        else if (state == 1 && sum >= 4) {
          next[i][j].state = 0;
          all_dead = false;
        }
        // Each cell with two or three neighbors survives.
        else if (state == 1 && (sum == 2 || sum == 3)) {
          next[i][j].state = 1;
          all_dead = false;
        }
        // Each cell with three neighbors becomes populated.
        else if (state == 0 && sum == 3) {
          next[i][j].state = 1;
          all_dead = false;
        }
      }
    }
    grids = next;

    if (all_dead) {
      start = false;
    }
  }

  void ToggleState(int x, int y) {
    if (start) return;
    if (all_dead) {
      // if all grid dead, then set generation to zero
      // set all dead to false
      // set thickness to 1.0f
      // no need to call Init
      cycle = 0;
      all_dead = false;
      thickness = 1.0f;
    }

    int x_ = x + render_x;
    int y_ = y + render_y;
    int row = y_ / size;
    int col = x_ / size;

    grids[row][col].state = grids[row][col].state == 1 ? 0 : 1;
  }

  void AddSize(int size) {
    this->size += size;
    this->thickness += size * 0.05;
    this->thickness = this->thickness < 0.5 ? 0.5 : this->thickness;
  }
  void MoveStartX(int dx) { this->move_x -= dx; }
  void MoveStartY(int dy) { this->move_y -= dy; }
  bool AllDead() { return all_dead; }
  bool Start() { return start; }
  void Start(bool start) { this->start = start; }

 private:
  std::vector<std::vector<Grid>> grids;
  float size;      ///< grid's size
  float render_x;  ///< start x coordinate for rendering
  float render_y;  ///< start y coordinate for rendering
  float move_x;    ///< moved x, start_x+move_x will be final coordinate x for
                   ///< rendering
  float move_y;    ///< moved y, start_y+move_y will be final coordinate y for
                   ///< rendering
  int padding;     ///< padding between grid and background
  int rows;        ///< rows number of grids
  int cols;        ///< cols number of grids
  float width_ratio;   ///< width ratio, window's width * width_raio will be
                       ///< grid's render width
  float height_ratio;  ///< height ratio, window's height * height_raio will be
                       ///< grid's render height
  long cycle;          ///< current generation
  float thickness;     ///< line thickness
  bool all_dead;       ///< no existing object
  bool start;          ///< check if game is started
};
