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

struct Spherical {
  float r;
  float theta;
  float phi;

  Spherical(float r, float theta, float phi) : r(r), theta(theta), phi(phi) {}

  static Spherical From(float x, float y, float z) {
    float r = sqrt(x * x + y * y + z * z);
    float theta = atan2(sqrt(x * x + y * y), z);
    float phi = atan2(y, x);
    return {r, theta, phi};
  }
};

class Mandelbulb {
 public:
  Mandelbulb() : points({}) { Init(); }

  void Init() {
    points.clear();
    for (float i = 0; i < 1000; i += 1) {
      for (float j = 0; j < 1000; j += 1) {
        bool edge = false;
        for (float k = 0; k < 1000; k += 1) {
          float x = -1.0f + i / 100 * 2.0f;
          float y = -1.0f + j / 100 * 2.0f;
          float z = -1.0f + k / 100 * 2.0f;

          int max_iter = 200;
          int iteration = 0;
          int n = 16;

          glm::vec3 zeta(0);

          while (true) {
            Spherical spherical_z = Spherical::From(zeta.x, zeta.y, zeta.z);
            float newx = pow(spherical_z.r, n) * sin(spherical_z.theta * n) *
                         cos(spherical_z.phi * n);
            float newy = pow(spherical_z.r, n) * sin(spherical_z.theta * n) *
                         sin(spherical_z.phi * n);
            float newz = pow(spherical_z.r, n) * cos(spherical_z.theta * n);

            zeta.x = newx + x;
            zeta.y = newy + y;
            zeta.z = newz + z;

            iteration++;

            if (spherical_z.r > 2) {
              if (edge) {
                edge = false;
              }
              break;
            }

            if (iteration > max_iter) {
              if (!edge) {
                edge = true;
                points.push_back(x);
                points.push_back(y);
                points.push_back(z);
                points.push_back(i / 100);
                points.push_back(j / 100);
                points.push_back(k / 100);
              }
              break;
            }
          }
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

    glm::mat4 proj =
        glm::perspective(glm::radians(camera.zoom), 16.0f / 9.0f, 0.1f, 100.0f);

    shader.SetUniformMat4f("model", model);
    shader.SetUniformMat4f("view", camera.GetViewMatrix());
    shader.SetUniformMat4f("projection", proj);

    renderer.DrawWithoutIBO(va, shader, points.size());
  }

 private:
  std::vector<float> points;
  float real_start, real_end, imag_start, imag_end;
};
