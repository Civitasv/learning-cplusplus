#pragma once
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
        start_x(0),
        start_y(0),
        padding(10),
        width_ratio(0.7),
        height_ratio(1.0),
        cycle(0) {
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
  }

  void Render(RayWindow& window) {
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
      // Render current cycle
      std::string text = "Current generation: ";
      std::string cycle_str = std::to_string(cycle);
      DrawText((text + cycle_str).c_str(), window.GetW() * 0.7 + padding,
               window.GetH() * 0.7, 20, RAYWHITE);
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
          int x = start_x + item.col * size;
          int y = start_y + item.row * size;
          if (!(x >= padding && (x + size) <= width - padding && y >= padding &&
                (y + size) <= height - padding)) {
            continue;
          }

          DrawRectangleLinesEx({start_x + item.col * size,
                                start_y + item.row * size, size, size},
                               1, state_1);
          if (item.state == 1)
            DrawRectangle(start_x + item.col * size + 1,
                          start_y + item.row * size + 1, size - 2, size - 2,
                          state_2);
        }
      }
    }
  }

  void Next() {
    cycle += 1;
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
        }
        // Each cell with four or more neighbors dies, as if by overpopulation.
        else if (state == 1 && sum >= 4) {
          next[i][j].state = 0;
        }
        // Each cell with two or three neighbors survives.
        else if (state == 1 && (sum == 2 || sum == 3)) {
          next[i][j].state = 1;
        }
        // Each cell with three neighbors becomes populated.
        else if (state == 0 && sum == 3) {
          next[i][j].state = 1;
        }
      }
    }
    grids = next;
  }

  void ToggleState(int x, int y) {
    int x_ = x - start_x;
    int y_ = y - start_y;
    int row = y_ / size;
    int col = x_ / size;

    grids[row][col].state = grids[row][col].state == 1 ? 0 : 1;
  }

  void AddSize(int size) { this->size += size; }
  void MoveStartX(int dx) { this->start_x += dx; }
  void MoveStartY(int dy) { this->start_y += dy; }

 private:
  std::vector<std::vector<Grid>> grids;
  float size;
  float start_x;
  float start_y;
  int padding;
  int rows;
  int cols;
  float width_ratio;
  float height_ratio;
  long cycle;
};
