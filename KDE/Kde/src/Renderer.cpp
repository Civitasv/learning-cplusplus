#include "Renderer.h"

#include <chrono>
#include <iostream>

#include "Log.h"
#include "bs_thread_pool/BS_thread_pool.hpp"
#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"

void Renderer::Clear() const {
  GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void Renderer::Draw(RendererElement& element, bool has_IBO) const {
  element.shader->Bind();

  // I can just bind VAO, it will bind VBO and vertex layout and IBO for us.
  element.va->Bind();

  if (has_IBO) {
    GLCall(glDrawElements(GL_POINTS, element.ib->GetCount(), GL_UNSIGNED_INT,
                          nullptr));
  } else {
    GLCall(glDrawArrays(GL_POINTS, 0, element.vb->GetCount()));
  }
}

RendererElement Renderer::PrepareData(kde::KDEResult* res) const {
  using namespace std::chrono;
  auto start = high_resolution_clock::now();
  int cols = res->cols, rows = res->rows;

  std::vector<float> data(cols * rows * 5);
  std::vector<unsigned int> indices(cols * rows);

  int count = 0;
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      int index = i * cols + j;
      int count = index * 5;
      data[count + 0] = j;
      data[count + 1] = i;
      float val = (res->estimate[i][j] - res->min) / (res->max - res->min);

      if (val == 0) {
        data[count + 2] = 1;
        data[count + 3] = 1;
        data[count + 4] = 1;
      } else if (val > 0 && val < 0.4) {
        data[count + 2] = 1;
        data[count + 3] = 1;
        data[count + 4] = 0;
      } else if (val >= 0.4 && val < 0.7) {
        data[count + 2] = 0;
        data[count + 3] = 1;
        data[count + 4] = 0;
      } else if (val >= 0.7 && val < 0.9) {
        data[count + 2] = 0;
        data[count + 3] = 0;
        data[count + 4] = 1;
      } else if (val >= 0.9 && val <= 1) {
        data[count + 2] = 1;
        data[count + 3] = 0;
        data[count + 4] = 0;
      } else {
        data[count + 2] = 1;
        data[count + 3] = 1;
        data[count + 4] = 1;
      }

      indices[index] = index;
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

RendererElement Renderer::PrepareDataMultiThread(kde::KDEResult* res) const {
  using namespace std::chrono;
  auto start = high_resolution_clock::now();
  int cols = res->cols, rows = res->rows;

  std::vector<float> data(cols * rows * 5);
  std::vector<unsigned int> indices(cols * rows);

  BS::thread_pool pool;

  auto loop = [&res, &data, &indices, rows, cols](const int a, const int b) {
    for (int i = a; i < b; i++) {
      for (int j = 0; j < cols; j++) {
        int index = i * cols + j;
        int count = index * 5;
        data[count + 0] = j;
        data[count + 1] = i;
        float val = (res->estimate[i][j] - res->min) / (res->max - res->min);

        if (val == 0) {
          data[count + 2] = 1;
          data[count + 3] = 1;
          data[count + 4] = 1;
        } else if (val > 0 && val < 0.4) {
          data[count + 2] = 1;
          data[count + 3] = 1;
          data[count + 4] = 0;
        } else if (val >= 0.4 && val < 0.7) {
          data[count + 2] = 0;
          data[count + 3] = 1;
          data[count + 4] = 0;
        } else if (val >= 0.7 && val < 0.9) {
          data[count + 2] = 0;
          data[count + 3] = 0;
          data[count + 4] = 1;
        } else if (val >= 0.9 && val <= 1) {
          data[count + 2] = 1;
          data[count + 3] = 0;
          data[count + 4] = 0;
        }

        indices[index] = index;
      }
    }
  };
  pool.parallelize_loop(0, rows, loop).wait();

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
