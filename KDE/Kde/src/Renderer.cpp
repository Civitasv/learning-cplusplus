#include "Renderer.h"

#include <chrono>
#include <iostream>

#include "Log.h"
#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"

void Renderer::Clear() const {
  GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void Renderer::Draw(RendererElement& element) const {
  element.shader->Bind();

  // I can just bind VAO, it will bind VBO and vertex layout and IBO for us.
  element.va->Bind();

  GLCall(glDrawElements(GL_POINTS, element.ib->GetCount(), GL_UNSIGNED_INT,
                        nullptr));
}

RendererElement Renderer::PrepareData(kde::KDEResult* res) const {
  using namespace std::chrono;
  auto start = high_resolution_clock::now();
  std::vector<float> data;
  std::vector<unsigned int> indices;
  int count = 0;
  for (int i = 0; i < res->rows; i++) {
    for (int j = 0; j < res->cols; j++) {
      data.push_back(j);
      data.push_back(i);
      float val = (res->estimate[i][j] - res->min) / (res->max - res->min);

      if (val == 0) {
        data.push_back(1);
        data.push_back(1);
        data.push_back(1);
      } else if (val > 0 && val < 0.4) {
        data.push_back(1);
        data.push_back(1);
        data.push_back(0);
      } else if (val >= 0.4 && val < 0.7) {
        data.push_back(0);
        data.push_back(1);
        data.push_back(0);
      } else if (val >= 0.7 && val < 0.9) {
        data.push_back(0);
        data.push_back(0);
        data.push_back(1);
      } else if (val >= 0.9 && val <= 1) {
        data.push_back(1);
        data.push_back(0);
        data.push_back(0);
      } else {
        data.push_back(1);
        data.push_back(1);
        data.push_back(1);
      }

      indices.push_back(count++);
    }
  }

  // vertex array object
  VertexArray* va = new VertexArray();
  // vertex buffer object
  VertexBuffer* vb = new VertexBuffer(data.data(), data.size() * sizeof(float));
  // specify layout in vertex buffer
  VertexBufferLayout layout;
  layout.Push<float>(2);
  layout.Push<float>(3);

  // index buffer object, specify how I want to draw this layout.
  IndexBuffer* ib = new IndexBuffer(indices.data(), indices.size());

  va->AddBuffer(*vb, *ib, layout);

  // 4 * 3
  float height = res->rows, width = res->cols;

  glm::mat4 proj = glm::ortho(0.0f, width, 0.0f, height, -1.0f, 1.0f);

  Shader* shader = new Shader("res/shaders/Basic.shader");
  shader->Bind();
  shader->SetUniformMat4f("u_MVP", proj);

  // unbind everything
  shader->Unbind();
  va->Unbind();
  vb->Unbind();
  ib->Unbind();

  auto stop = high_resolution_clock::now();
  auto duration = duration_cast<milliseconds>(stop - start);
  std::cout << "PREPARE DATA TIME:: " << duration.count() << " ms" << std::endl;
  return {shader, va, vb, ib};
}
