#pragma once

#include <glad/glad.h>

#include <memory>
#include <string>
#include <vector>
#include <utility>

#include "shader.hpp"

class ShaderProgram {
public:
  ShaderProgram() = delete;
  ShaderProgram(const std::string &vertex_filename, const std::string &fragment_filename);
  ShaderProgram(const std::string &vertex_filename, const std::string &fragment_filename, std::vector<GLuint> positions, std::vector<std::string> names);

  GLint get_attribute_location(const std::string &attribute_name);
  GLint get_uniform_location(const std::string &uniform_name);

  void use();

  ~ShaderProgram();

private:
  GLuint program_id = 0;
  std::unique_ptr<VertexShader> vertex_shader;
  std::unique_ptr<FragmentShader> fragment_shader;
};
