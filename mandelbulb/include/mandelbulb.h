#pragma once
#include <glad/gl.h>

#include <algorithm>
#include <complex>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

#include "GLFW/glfw3.h"
#include "camera.h"
#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"
#include "index_buffer.h"
#include "renderer.h"
#include "texture.h"
#include "vertex_array.h"
#include "vertex_buffer.h"
#include "vertex_buffer_layout.h"

class Mandelbulb {
 public:
  Mandelbulb() : points({}) { Init(); }

  void Init() {
    points.clear();
    for (float i = 0; i < 100; i += 1) {
      for (float j = 0; j < 100; j += 1) {
        for (float k = 0; k < 100; k += 1) {
          points.push_back(i);
          points.push_back(j);
          points.push_back(k);
          points.push_back(1.0f);
          points.push_back(1.0f);
          points.push_back(1.0f);
        }
      }
    }
  }

  void Calculate(float real_start, float real_end, float imag_start,
                 float imag_end) {}

  void Render(Renderer& renderer, Camera& camera, float angle_x, float angle_y,
              float angle_z) {
    // vertex array object
    VertexArray va;
    // vertex buffer object
    VertexBuffer vb(&points[0], points.size() * sizeof(float));
    // specify layout in vertex buffer
    VertexBufferLayout layout;
    layout.Push<float>(3);  // pos
    layout.Push<float>(3);  // color

    va.AddBuffer(vb, layout);

    Shader shader("res/shaders/Mandelbulb.shader");

    shader.Bind();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(angle_x),
                        glm::vec3(1.0f, 0.0f, 0.0f));  // ÈÆxÖáÐý×ª
    model = glm::rotate(model, glm::radians(angle_y),
                        glm::vec3(0.0f, 1.0f, 0.0f));  // ÈÆyÖáÐý×ª
    model = glm::rotate(model, glm::radians(angle_z),
                        glm::vec3(0.0f, 0.0f, 1.0f));  // ÈÆzÖáÐý×ª

    glm::mat4 proj = glm::perspective(glm::radians(camera.zoom),
                                      640.0f / 640.0f, 0.1f, 400.0f);

    shader.SetUniformMat4f("model", model);
    shader.SetUniformMat4f("view", camera.GetViewMatrix());
    shader.SetUniformMat4f("projection", proj);

    renderer.DrawWithoutIBO(va, shader, points.size());
  }

 private:
  std::vector<float> points;
  float real_start, real_end, imag_start, imag_end;
  int max_iter;
};
