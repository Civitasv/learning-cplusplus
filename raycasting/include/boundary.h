#pragma once
#include "point.h"
#include "ray_window.h"

class Boundary {
 public:
  Boundary(Point start, Point end) : start(start), end(end), border(false) {}
  Boundary(Point start, Point end, bool border)
      : start(start), end(end), border(border) {}
  void Render() {
    if (!border) DrawLine(start.x, start.y, end.x, end.y, WHITE);
  }

  Point start;
  Point end;
  bool border;
};