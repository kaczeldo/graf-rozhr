#include "program.hpp"

#include <vector>

ShaderProgram::ShaderProgram(const std::string &vertex_filename, const std::string &fragment_filename) {
  this->vertex_shader = std::make_unique<VertexShader>(vertex_filename);
  if (this->vertex_shader->get_compilation_status() == ShaderCompilationStatus::Failure) {
    const std::string log = this->vertex_shader->get_log();
  }

  this->fragment_shader = std::make_unique<FragmentShader>(fragment_filename);
  if (this->fragment_shader->get_compilation_status() == ShaderCompilationStatus::Failure) {
    const std::string log = this->fragment_shader->get_log();
  }

  this->program_id = glCreateProgram();
  glAttachShader(this->program_id, vertex_shader->get_id());
  glAttachShader(this->program_id, fragment_shader->get_id());
  glLinkProgram(this->program_id);

  int compile_status = GL_FALSE;
  glGetProgramiv(this->program_id, GL_LINK_STATUS, &compile_status);
  if (compile_status == GL_FALSE) {
    // Retrieve length of the information log
    int log_len = 0;
    glGetProgramiv(this->program_id, GL_INFO_LOG_LENGTH, &log_len);

    // Allocate
    std::vector<char> log(log_len);
    glGetProgramInfoLog(this->program_id, log_len, nullptr, log.data());

    // std::string(begin(log), end(log));
  }
}

ShaderProgram::ShaderProgram(const std::string &vertex_filename, const std::string &fragment_filename, std::vector<GLuint> positions, std::vector<std::string> names) {
    this->vertex_shader = std::make_unique<VertexShader>(vertex_filename);
    if (this->vertex_shader->get_compilation_status() == ShaderCompilationStatus::Failure) {
      const std::string log = this->vertex_shader->get_log();
    }

    this->fragment_shader = std::make_unique<FragmentShader>(fragment_filename);
    if (this->fragment_shader->get_compilation_status() == ShaderCompilationStatus::Failure) {
      const std::string log = this->fragment_shader->get_log();
    }

    this->program_id = glCreateProgram();
    glAttachShader(this->program_id, vertex_shader->get_id());
    glAttachShader(this->program_id, fragment_shader->get_id());

    for(uint32_t i = 0; i < positions.size(); i++) {
        glBindAttribLocation(this->program_id, positions[i], names[i].c_str());
    }

    glLinkProgram(this->program_id);

    int compile_status = GL_FALSE;
    glGetProgramiv(this->program_id, GL_LINK_STATUS, &compile_status);
    if (compile_status == GL_FALSE) {
      // Retrieve length of the information log
      int log_len = 0;
      glGetProgramiv(this->program_id, GL_INFO_LOG_LENGTH, &log_len);

      // Allocate
      std::vector<char> log(log_len);
      glGetProgramInfoLog(this->program_id, log_len, nullptr, log.data());

      // std::string(begin(log), end(log));
    }
}

GLint ShaderProgram::get_attribute_location(const std::string &attribute_name) {
  return glGetAttribLocation(this->program_id, attribute_name.c_str());
}

GLint ShaderProgram::get_uniform_location(const std::string &uniform_name) {
  return glGetUniformLocation(this->program_id, uniform_name.c_str());
}

void ShaderProgram::use() { glUseProgram(this->program_id); }

ShaderProgram::~ShaderProgram() { glDeleteProgram(program_id); }
