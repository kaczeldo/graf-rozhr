#pragma once

#include <glad/glad.h>

#include <string>

enum class ShaderCompilationStatus { Success, Failure };

class Shader {
public:
  Shader() = delete;
  Shader(const std::string &file_name) {}
  virtual ~Shader() { glDeleteShader(this->shader_id); }

  ShaderCompilationStatus get_compilation_status();
  const std::string get_log();
  GLuint get_id() { return shader_id; }

protected:
  GLuint shader_id = 0;
};

class VertexShader : public Shader {
public:
  VertexShader() = delete;
  VertexShader(const std::string &file_name);
};

class FragmentShader : public Shader {
public:
  FragmentShader() = delete;
  FragmentShader(const std::string &file_name);
};
