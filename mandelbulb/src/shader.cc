#include "shader.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "glad/gl.h"
#include "log.h"

Shader::Shader(const std::string& filepath)
    : m_Filepath(filepath), m_RendererID(0) {
  ShaderProgramSource source = ParseShader(filepath);

  unsigned int program =
      CreateShader(source.VertexSource, source.FragmentSource);
  m_RendererID = program;
}

Shader::~Shader() { GLCall(glDeleteProgram(m_RendererID)); }

void Shader::Bind() const { GLCall(glUseProgram(m_RendererID)); }

void Shader::Unbind() const { GLCall(glUseProgram(0)); }

void Shader::SetUniform1i(const std::string& name, int value) {
  int location = GetUniformLocation(name);
  GLCall(glUniform1i(location, value));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2,
                          float v3) {
  int location = GetUniformLocation(name);
  GLCall(glUniform4f(location, v0, v1, v2, v3));
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix) {
  int location = GetUniformLocation(name);
  GLCall(glUniformMatrix4fv(location, 1, GL_FALSE, &matrix[0][0]));
}

int Shader::GetUniformLocation(const std::string& name) {
  if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end()) {
    return m_UniformLocationCache[name];
  }

  GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
  if (location == -1) {
    std::cout << "Warning: uniform '" << name << "' doesn't exist!"
              << std::endl;
  }
  m_UniformLocationCache[name] = location;

  return location;
}

ShaderProgramSource Shader::ParseShader(const std::string& filepath) {
  std::ifstream stream(filepath);
  enum class ShaderType { NONE = -1, VERTEX = 0, FRAGMENT = 1 };
  std::string line;
  std::stringstream ss[2];
  ShaderType type = ShaderType::NONE;

  while (getline(stream, line)) {
    if (line.find("#shader") != std::string::npos) {
      if (line.find("vertex") != std::string::npos) {
        // vertex
        type = ShaderType::VERTEX;
      } else if (line.find("fragment") != std::string::npos) {
        // fragment
        type = ShaderType::FRAGMENT;
      }
    } else {
      ss[(int)type] << line << '\n';
    }
  }
  return {ss[0].str(), ss[1].str()};
}

unsigned int Shader::CompileShader(unsigned int type,
                                   const std::string& source) {
  GLCall(unsigned int id = glCreateShader(type));
  const char* src = source.c_str();
  // Set the source code in shader(id) to the source code in the array of
  // strings specified by string(source).
  GLCall(glShaderSource(id, 1, &src, nullptr));
  // Compile the source code strings that have been stored in the shader object
  // specified by shader.
  GLCall(glCompileShader(id));

  // GET ERROR MESSAGE
  int result;
  GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
  if (result == GL_FALSE) {
    int length;
    GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
    char* message = (char*)alloca(length * sizeof(char));
    GLCall(glGetShaderInfoLog(id, length, &length, message));
    std::cout << "FAILED to compile" << source << " shader!" << message << '\n';

    GLCall(glDeleteShader(id));
    return 0;
  }
  return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShader,
                                  const std::string& fragmentShader) {
  GLCall(unsigned int program = glCreateProgram());
  unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
  unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

  GLCall(glAttachShader(program, vs));
  GLCall(glAttachShader(program, fs));

  GLCall(glLinkProgram(program));
  GLCall(glValidateProgram(program));

  // vs and fs kinda like C++'s object file, so after linking, we can delete
  // them.
  GLCall(glDeleteShader(vs));
  GLCall(glDeleteShader(fs));

  return program;
}