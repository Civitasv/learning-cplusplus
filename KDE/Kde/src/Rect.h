#pragma once

namespace kde {
class Rect {
 public:
  Rect() : top(-INFINITY), right(-INFINITY), bottom(INFINITY), left(INFINITY) {}
  Rect(float top, float right, float bottom, float left)
      : top(top), right(right), bottom(bottom), left(left) {}

  float top;
  float right;
  float bottom;
  float left;
};
}  // namespace kde