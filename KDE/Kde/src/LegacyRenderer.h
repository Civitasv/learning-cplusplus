#pragma once
#include "GLFW/glfw3.h"
#include "kde.h"

class LegacyRenderer {
 private:
 public:
  void Clear() const;
  void Draw(kde::KDEResult* res) const;
};