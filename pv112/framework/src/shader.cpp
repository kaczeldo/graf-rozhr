#include "shader.hpp"
#include "utility.hpp"

#include <memory>
#include <vector>

ShaderCompilationStatus Shader::get_compilation_status() {
  GLint compile_status;
  glGetShaderiv(this->shader_id, GL_COMPILE_STATUS, &compile_status);

  return compile_status == GL_TRUE ? ShaderCompilationStatus::Success : ShaderCompilationStatus::Failure;
}

const std::string Shader::get_log() {
  // Retrieve length of the information log
  int log_len = 0;
  glGetShaderiv(this->shader_id, GL_INFO_LOG_LENGTH, &log_len);

  // Allocate
  std::vector<char> log(log_len);
  glGetShaderInfoLog(this->shader_id, log_len, nullptr, log.data());

  return std::string(begin(log), end(log));
}

VertexShader::VertexShader(const std::string &file_name) : Shader(file_name) {
  // Load file into string
  const std::string source = load_file_to_string(file_name);

  // Generate new ID
  this->shader_id = glCreateShader(GL_VERTEX_SHADER);

  // Load source into OpenGL driver
  const char *sources = source.c_str();
  glShaderSource(this->shader_id, 1, &sources, nullptr);

  // Ask for compilation
  glCompileShader(this->shader_id);
}

FragmentShader::FragmentShader(const std::string &file_name) : Shader(file_name) {
  // Load file into string
  const std::string source = load_file_to_string(file_name);

  // Generate new ID
  this->shader_id = glCreateShader(GL_FRAGMENT_SHADER);

  // Load source into OpenGL driver
  const char *sources = source.c_str();
  glShaderSource(this->shader_id, 1, &sources, nullptr);

  // Ask for compilation
  glCompileShader(this->shader_id);
}