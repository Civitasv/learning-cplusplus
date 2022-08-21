#pragma once

class Food {
 public:
  int x;
  int y;
  int size;

  Food() : x(-1), y(-1), size(0) {}
  Food(int x, int y, int size) : x(x), y(y), size(size) {}
};