#include "LegacyRenderer.h"

#include <iostream>

#include "GLFW/glfw3.h"
#include "Log.h"
#include "kde.h"

using namespace kde;

void LegacyRenderer::Clear() const {
  GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void LegacyRenderer::Draw(KDEResult* res) const {
  float max = res->max, min = res->min;
  int height = res->rows, width = res->cols;
  GLCall(glMatrixMode(GL_PROJECTION));
  // 加载单位矩阵
  GLCall(glLoadIdentity());
  GLCall(glOrtho(0, width, 0, height, -1, 1));
  GLCall(glBegin(GL_POINTS));
  for (int x = 0; x < width; x++) {
    for (int y = 0; y < height; y++) {
      float val = (res->estimate[y][x] - min) / (max - min);
      if (val == 0) {
        GLCall(glColor3d(1, 1, 1));
      } else if (val > 0 && val < 0.4) {
        GLCall(glColor3d(1, 1, 0));
      } else if (val >= 0.4 && val < 0.7) {
        GLCall(glColor3d(0, 1, 0));
      } else if (val >= 0.7 && val < 0.9) {
        GLCall(glColor3d(0, 0, 1));
      } else if (val >= 0.9 && val <= 1) {
        GLCall(glColor3d(1, 0, 0));
      }
      GLCall(glVertex2d(x, y));
    }
  }
  GLCall(glEnd());
  GLCall(glPopMatrix());
  // 刷新
  GLCall(glFlush());
}