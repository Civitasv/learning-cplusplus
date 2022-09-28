#include "VertexBuffer.h"

#include "Log.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size) {
  GLCall(glGenBuffers(1, &m_RendererID));
  GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
  GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

// GPU BASED
VertexBuffer::VertexBuffer(unsigned int size, unsigned int count)
    : m_Count(count) {
  GLCall(glGenBuffers(1, &m_RendererID));
  GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
  GLCall(glBufferData(GL_ARRAY_BUFFER, size, 0, GL_DYNAMIC_DRAW_ARB));
  cudaGraphicsGLRegisterBuffer(&vbo_cuda, m_RendererID,
                               cudaGraphicsMapFlagsNone);
  // make cuda can access vbo_cuda
  cudaGraphicsMapResources(1, &vbo_cuda, NULL);
}

VertexBuffer::~VertexBuffer() { GLCall(glDeleteBuffers(1, &m_RendererID)); }

void VertexBuffer::Bind() const {
  GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

void VertexBuffer::Unbind() const { GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0)); }

unsigned int VertexBuffer::GetCount() const { return m_Count; }
