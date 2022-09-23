#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

//#define LEGACY_OPENGL

#ifdef LEGACY_OPENGL
#include "LegacyRenderer.h"
#else
#include "Renderer.h"
#endif  // !LEGACY_OPENGL

int main(void) {
  GLFWwindow* window;

  /* Initialize the library */
  if (!glfwInit()) return -1;

#ifndef LEGACY_OPENGL
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

  /* Create a windowed mode window and its OpenGL context */
  window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
  if (!window) {
    glfwTerminate();
    return -1;
  }

  /* Make the window's context current */
  glfwMakeContextCurrent(window);

#ifndef LEGACY_OPENGL
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
#endif

  {
    using namespace kde;
    using namespace std::chrono;

    KDEResult* res = Calculate();

#ifdef LEGACY_OPENGL
    LegacyRenderer renderer;
#else
    Renderer renderer;
    RendererElement element = renderer.PrepareData(res);
#endif

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
      /* Render here */
      renderer.Clear();
      auto start = high_resolution_clock::now();

#ifdef LEGACY_OPENGL
      renderer.Draw(res);
#else
      renderer.Draw(element);
#endif
      auto stop = high_resolution_clock::now();
      auto duration = duration_cast<microseconds>(stop - start);
      std::cout << "RENDER TIME:: " << duration.count() << std::endl;
      /* Swap front and back buffers */
      glfwSwapBuffers(window);
      glfwSwapInterval(1);

      /* Poll for and process events */
      glfwPollEvents();
    }
  }
  glfwTerminate();
  return 0;
}