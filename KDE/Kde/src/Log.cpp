#include "Log.h"

#include <iostream>

#include "glad/gl.h"

void GLClearError() {
  while (glGetError())
    ;
}

bool GLLogCall(const char* function, const char* file, int line) {
  while (GLenum error = glGetError()) {
    std::cout << "[OpenGL Error] (" << error << "): " << function << " " << file
              << ": " << line << std::endl;
    return false;
  }

  return true;
}