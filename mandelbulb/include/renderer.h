#pragma once

#include "shader.h"
#include "vertex_array.h"

class Renderer {
 private:
 public:
  void Clear() const;
  void Draw(const VertexArray& va, const Shader& shader, int count) const;
  void DrawWithoutIBO(const VertexArray& va, const Shader& shader, int count) const;
};