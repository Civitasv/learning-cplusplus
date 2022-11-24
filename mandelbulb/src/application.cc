#include <glad/gl.h>

#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "GLFW/glfw3.h"
#include "camera.h"
#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"
#include "index_buffer.h"
#include "mandelbulb.h"
#include "renderer.h"
#include "texture.h"
#include "vertex_array.h"
#include "vertex_buffer.h"
#include "vertex_buffer_layout.h"

// callback
void processInput(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 5.0f));
float lastX = 300, lastY = 300;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;  // Time between current frame and last frame
float lastFrame = 0.0f;  // Time of last frame

// rotation
float angle_x = 0.0f;
float angle_y = 0.0f;
float angle_z = 0.0f;
enum RotationMode { X, Y, Z };
RotationMode rotation_mode = X;

// Mandelbulb
Mandelbulb mandelbulb;

int main(void) {
  GLFWwindow* window;

  /* Initialize the library */
  if (!glfwInit()) return -1;

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  /* Create a windowed mode window and its OpenGL context */
  window = glfwCreateWindow(1280, 720, "Mandelbulb", NULL, NULL);
  if (!window) {
    glfwTerminate();
    return -1;
  }

  /* Make the window's context current */
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  // glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetScrollCallback(window, scroll_callback);
  // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  // init glad
  // Load OpenGL functions, gladLoadGL returns the loaded version, 0 on error.
  int version = gladLoadGL(glfwGetProcAddress);
  if (version == 0) {
    std::cout << "Failed to initialize OpenGL context" << std::endl;
    return -1;
  }
  // Successfully loaded OpenGL
  std::cout << "Loaded OpenGL " << GLAD_VERSION_MAJOR(version) << "."
            << GLAD_VERSION_MINOR(version) << std::endl;

  {
    GLCall(glEnable(GL_DEPTH_TEST));
    Renderer renderer;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
      float currentFrame = glfwGetTime();
      deltaTime = currentFrame - lastFrame;
      lastFrame = currentFrame;

      processInput(window);

      /* Render here */
      renderer.Clear();
      mandelbulb.Render(renderer, camera, angle_x, angle_y, angle_z);

      /* Swap front and back buffers */
      glfwSwapBuffers(window);
      glfwSwapInterval(1);

      /* Poll for and process events */
      GLCall(glfwPollEvents());
    }
  }
  glfwTerminate();
  return 0;
}

void processInput(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    camera.ProcessKeyboard(FORWARD, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    camera.ProcessKeyboard(BACKWARD, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    camera.ProcessKeyboard(LEFT, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    camera.ProcessKeyboard(RIGHT, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    camera.ProcessKeyboard(UP, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    camera.ProcessKeyboard(DOWN, deltaTime);

  if (glfwGetKey(window, GLFW_KEY_X)) {
    // rotate left
    rotation_mode = X;
  }
  if (glfwGetKey(window, GLFW_KEY_Y)) {
    // rotate left
    rotation_mode = Y;
  }
  if (glfwGetKey(window, GLFW_KEY_Z)) {
    // rotate left
    rotation_mode = Z;
  }
  if (glfwGetKey(window, GLFW_KEY_Q)) {
    if (rotation_mode == X) {
      angle_x -= 1.0f;
    } else if (rotation_mode == Y) {
      angle_y -= 1.0f;
    } else if (rotation_mode == Z) {
      angle_z -= 1.0f;
    }
  }
  if (glfwGetKey(window, GLFW_KEY_E)) {
    if (rotation_mode == X) {
      angle_x += 1.0f;
    } else if (rotation_mode == Y) {
      angle_y += 1.0f;
    } else if (rotation_mode == Z) {
      angle_z += 1.0f;
    }
  }
}

// glfw: whenever the window size changed (by OS or user resize) this callback
// function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
  // make sure the viewport matches the new window dimensions; note that width
  // and height will be significantly larger than specified on retina displays.
  glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
  float xpos = static_cast<float>(xposIn);
  float ypos = static_cast<float>(yposIn);

  if (firstMouse) {
    lastX = xpos;
    lastY = ypos;
    firstMouse = false;
  }

  float xoffset = xpos - lastX;
  float yoffset =
      lastY - ypos;  // reversed since y-coordinates go from bottom to top

  lastX = xpos;
  lastY = ypos;

  camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
  camera.ProcessMouseScroll(static_cast<float>(yoffset));
}