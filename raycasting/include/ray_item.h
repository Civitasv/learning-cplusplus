#pragma once

#include <iostream>

#include "boundary.h"
#include "point.h"
#include "ray_window.h"

class RayItem {
 public:
  RayItem(Point& p, float angle) : pos(p), angle(angle), direction(-1, -1) {
    direction.x = cos(angle);
    direction.y = sin(angle);
  }

  void Render() {
    DrawLine(pos.x, pos.y, pos.x + direction.x * 5, pos.y + direction.y * 5,
             WHITE);
  }

  /// Calculate intersection point of ray and boundary
  Point&& Cast(Boundary& boundary) {
    float x1 = boundary.start.x;
    float x2 = boundary.end.x;
    float y1 = boundary.start.y;
    float y2 = boundary.end.y;
    //DrawLine(pos.x,pos.y,x1,y1,ORANGE);

    float x3 = pos.x;
    float y3 = pos.y;
    float x4 = pos.x + direction.x*100;
    float y4 = pos.y + direction.y*100;
    //DrawLine(pos.x, pos.y, x4, y4, ORANGE);

    float denom = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
    if (denom == 0) {
      return {-1, -1};
    }
    float t = ((x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4)) / (denom * 1.0f);
    float u = -((x1 - x2) * (y1 - y3) - (y1 - y2) * (x1 - x3)) / (denom * 1.0f);

    if (t >= 0 && t <= 1 && u > 0) {
      return {x1 + t * (x2 - x1), y1 + t * (y2 - y1)};
    } else {
      return {-1, -1};
    }
  }

  Point& pos;
  Point direction;
  float angle;
};
