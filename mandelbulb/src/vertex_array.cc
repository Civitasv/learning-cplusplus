#include "vertex_array.h"

#include "index_buffer.h"
#include "log.h"
#include "glad/gl.h"

VertexArray::VertexArray() { GLCall(glGenVertexArrays(1, &m_RendererID)); }

VertexArray::~VertexArray() { GLCall(glDeleteVertexArrays(1, &m_RendererID)); }

void VertexArray::AddBuffer(const VertexBuffer& vb, const IndexBuffer& ib,
                            const VertexBufferLayout& layout) {
  Bind();
  vb.Bind();
  ib.Bind();

  const auto& elements = layout.GetElements();
  unsigned int offset = 0;
  for (unsigned int i = 0; i < elements.size(); i++) {
    GLCall(glEnableVertexAttribArray(i));
    // index: index of this attribute
    // size: the number of components of this attribute
    // stride: byte offset between two attributes
    // pointer: first location of this attribute
    // 0 表示该属性的 index，2 表示该属性有两个数据组成，GL_FLOAT 表示每个数据是
    // float GL_FALSE 表示不要normalize，2*sizeof(float)
    // 表示该属性第一个值和第二个值之间的间隔， 0
    // 表示该属性第一个值在数据（positions）中的位置
    // 这个函数也使得 VBO 与 VAO 绑定
    GLCall(glVertexAttribPointer(i, elements[i].count, elements[i].type,
                                 elements[i].normalized, layout.GetStride(),
                                 (const void*)offset));
    offset += elements[i].count *
              VertexBufferElement::GetSizeOfType(elements[i].type);
  }
}

void VertexArray::AddBuffer(const VertexBuffer& vb,
                            const VertexBufferLayout& layout) {
  Bind();
  vb.Bind();

  const auto& elements = layout.GetElements();
  unsigned int offset = 0;
  for (unsigned int i = 0; i < elements.size(); i++) {
    GLCall(glEnableVertexAttribArray(i));
    GLCall(glVertexAttribPointer(i, elements[i].count, elements[i].type,
                                 elements[i].normalized, layout.GetStride(),
                                 (const void*)offset));
    offset += elements[i].count *
              VertexBufferElement::GetSizeOfType(elements[i].type);
  }
}

void VertexArray::Bind() const { GLCall(glBindVertexArray(m_RendererID)); }

void VertexArray::Unbind() const { GLCall(glBindVertexArray(0)); }
