#include "include/fractal.h"

static constexpr double PI = 3.1415926;

void Branch(RenderWindow& window, int x, int y, int len, int angle,
            int d_angle) {
  if (len <= 4) {
    return;
  }
  // root
  int dx = len * sin(angle / 180.0f * PI);
  int dy = len * cos(angle / 180.0f * PI);
  window.RenderLine(x, window.GetH() - y, x + dx, window.GetH() - (y + dy));
  // its branch
  // right
  Branch(window, x + dx, y + dy, len * 0.67, angle + d_angle, d_angle);
  // left
  Branch(window, x + dx, y + dy, len * 0.67, angle - d_angle, d_angle);
}
