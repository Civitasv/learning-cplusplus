#pragma once
#include <SDL2/SDL.h>

#include <vector>

#include "include/food.h"
#include "include/sdl_handler.h"

class Snake {
 public:
  Snake() : x(-1), y(-1), size(-1), x_step(0), y_step(0), total(0), tails({}) {}
  Snake(int x, int y, int size, SDL_Texture* header_tex, SDL_Texture* tails_tex,
        SDL_Handler* handler)
      : x(x),
        y(y),
        size(size),
        x_step(1),
        y_step(0),
        total(0),
        tails({}),
        header_tex(header_tex),
        tails_tex(tails_tex),
        handler(handler) {}

  int x;  // x location of snake
  int y;  // y location of snake

  int x_step;  // horizontal speed of this snake
  int y_step;  // vertical speed of this snake

  int size;
  int total;

  SDL_Texture* header_tex;
  SDL_Texture* tails_tex;

  SDL_Handler* handler;

  std::vector<std::pair<int, int>> tails;

  void Update() {
    if (total == tails.size()) {
      for (int i = 0; i < total - 1; i++) {
        tails[i] = tails[i + 1];
      }
      if (total > 0) {
        tails[total - 1] = {x, y};
      }
    } else {
      tails.push_back({x, y});
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
      return true;
    }
    return false;
  }

  bool Death(int rows, int cols) {
    // if touch itself
    for (auto& tail : tails) {
      if (tail.first == x && tail.second == y) {
        total = 0;
        tails.clear();
        return true;
      }
    }
    // if touch wall
    if (x == 0 || y == 0 || x == cols - 1 || y == rows - 1) {
      total = 0;
      tails.clear();
      x = 1;
      y = 1;
      x_step = 1;
      y_step = 0;
      return true;
    }

    return false;
  }

  void Render() {
    double angle = x_step == -1   ? 0
                   : x_step == 1  ? 180
                   : y_step == 1  ? 270
                   : y_step == -1 ? 90
                                  : 0;
    for (auto& item : tails) {
      SDL_Rect src{0, 0, 16, 16};

      SDL_Rect dst{handler->left + item.first * size,
                   handler->top + item.second * size, size, size};

      handler->DrawRectangle(src, dst, tails_tex, angle);
    }

    // render header
    SDL_Rect src = {0, 0, 16, 16};

    SDL_Rect dst{handler->left + x * size, handler->top + y * size, size, size};

    handler->DrawRectangle(src, dst, header_tex, angle);
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