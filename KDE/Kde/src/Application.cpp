#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

//#define LEGACY_OPENGL // if enable LEGACY OPENGL，如果开启，则使用 OPENGL
//立即绘制模式
#define KDE_USE_GPU_ACCELERATION  // if enable GPU Acceleration，包括使用 CUDA
// 并行计算核密度以及 CUDA 与 OPENGL 的互操作

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
  window = glfwCreateWindow(640, 640, "Hello World", NULL, NULL);
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

     auto start = high_resolution_clock::now();

#ifdef LEGACY_OPENGL
    KDEResult* res = CPUCalculate();
#elif defined KDE_USE_GPU_ACCELERATION
    RendererElement element = GPUCalculate();
#else
    KDEResult* res = CPUCalculate();
#endif
    //auto start = high_resolution_clock::now();
#ifdef LEGACY_OPENGL
    LegacyRenderer renderer;
#elif defined KDE_USE_GPU_ACCELERATION
    Renderer renderer;
#else
    Renderer renderer;
    RendererElement element = renderer.PrepareData(res);
#endif
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
    std::cout << "PREPARE TIME:: " << duration.count() << " ms" << std::endl;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
      auto start = high_resolution_clock::now();

      /* Render here */
      renderer.Clear();

#ifdef LEGACY_OPENGL
      renderer.Draw(res);
#elif defined KDE_USE_GPU_ACCELERATION
      renderer.Draw(element, false);
#else
      renderer.Draw(element, true);
#endif

      auto stop = high_resolution_clock::now();
      auto duration = duration_cast<microseconds>(stop - start);
      std::cout << "RENDER TIME:: " << duration.count() << " 微秒" << std::endl;
      /* Swap front and back buffers */
      glfwSwapBuffers(window);
      glfwSwapInterval(1);

      /* Poll for and process events */
      glfwPollEvents();
    }
#ifndef KDE_USE_GPU_ACCELERATION
    delete res;
#endif
  }
  glfwTerminate();
  return 0;
}