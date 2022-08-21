#pragma once
#include <SDL2/SDL.h>

#include <vector>

#include "include/food.h"

class Snake {
 public:
  Snake() : x(-1), y(-1), size(-1), x_step(0), y_step(0), total(0), tails({}) {}
  Snake(int x, int y, int size)
      : x(x), y(y), size(size), x_step(1), y_step(0), total(0), tails({}) {}
  int x;  // x location of snake
  int y;  // y location of snake

  int x_step;  // horizontal speed of this snake
  int y_step;  // vertical speed of this snake

  int size;
  int total;

  std::vector<std::pair<int, int>> tails;

  void Update() {
    for (int i = 0; i < total - 1; i++) {
      tails[i] = tails[i + 1];
    }
    if (total > 0) {
      tails[total - 1] = {x, y};
    }

    x = x + x_step;
    y = y + y_step;
  }

  void Dir(int x_dir, int y_dir) {
    // cannot reverse
    if (x_step + x_dir == 0) {
      return;
    }
    if (y_step + y_dir == 0) {
      return;
    }
    x_step = x_dir;
    y_step = y_dir;
  }

  bool Eat(Food& food) {
    if (x == food.x && y == food.y) {
      total++;
      tails.push_back({x, y});
      return true;
    }
    return false;
  }

  void Death(int rows, int cols) {
    // if touch itself
    for (auto& tail : tails) {
      if (tail.first == x && tail.second == y) {
        total = 0;
        tails.clear();
      }
    }
    // if touch wall
    if (x == -1 || y == -1 || x == cols || y == rows) {
      total = 0;
      tails.clear();
      x = 0;
      y = 0;
      x_step = 1;
      y_step = 0;
    }
  }

 private:
  int Constrain(int val, int start, int end) {
    if (val < start) {
      return start;
    }

    if (val > end) {
      return end;
    }
    return val;
  }
};