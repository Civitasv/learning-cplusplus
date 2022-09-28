#pragma once
#include "kde.h"

struct RendererElement;

namespace kde {
struct Item {
  int col;
  int row;
  float value;
};

RendererElement GPUCalculate();
}  // namespace kde