#pragma once
#include <glad/gl.h>
#include <cuda_gl_interop.h>

class VertexBuffer {
 private:
  unsigned int m_RendererID;
  unsigned int m_Count;

 public:
  VertexBuffer(unsigned int size, unsigned int count);

  VertexBuffer(const void* data, unsigned int size);

  ~VertexBuffer();

  void Bind() const;
  void Unbind() const;

  unsigned int GetCount() const;

  struct cudaGraphicsResource* vbo_cuda;
};