#pragma once
#include "index_buffer.h"
#include "vertex_buffer.h"
#include "vertex_buffer_layout.h"

class VertexArray {
 private:
  unsigned int m_RendererID;

 public:
  VertexArray();
  ~VertexArray();

  void AddBuffer(const VertexBuffer& vb, const IndexBuffer& ib,
                 const VertexBufferLayout& layout);
  void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
  void Bind() const;
  void Unbind() const;
};