#pragma once
#include "glad/gl.h"

#include "GLFW/glfw3.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"
#include "kde.h"

struct RendererElement {
  Shader* shader;
  VertexArray* va;
  VertexBuffer* vb;
  IndexBuffer* ib;

  ~RendererElement() {
    delete shader;
    delete va;
    delete vb;
    delete ib;
  }
};

class Renderer {
 private:
 public:
  void Clear() const;
  void Draw(RendererElement& element) const;
  RendererElement PrepareData(kde::KDEResult* res) const;
};