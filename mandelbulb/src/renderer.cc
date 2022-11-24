#include "renderer.h"

#include <chrono>
#include <iostream>

#include "bs_thread_pool/BS_thread_pool.hpp"
#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"
#include "log.h"

void Renderer::Clear() const {
  GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void Renderer::Draw(const VertexArray& va, const Shader& shader,
                    int count) const {
  shader.Bind();

  // I can just bind VAO, it will bind VBO and vertex layout and IBO for us.
  va.Bind();

  GLCall(glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr));
}

void Renderer::DrawWithoutIBO(const VertexArray& va, const Shader& shader,
                              int count) const {
  shader.Bind();

  // I can just bind VAO, it will bind VBO and vertex layout for us.
  va.Bind();

  GLCall(glDrawArrays(GL_POINTS, 0, count));
}