#pragma once

#include <glad/glad.h>

#include <vector>
#include <string>

class Mesh {
public:
  Mesh(const std::string &file_name, GLint position_location = -1, GLint normal_location = -1,
       GLint tex_coord_location = -1);

  Mesh(std::vector<float> vertices, std::vector<uint32_t> indices, GLenum mode = GL_TRIANGLES,
       GLint position_location = -1, GLint normal_location = -1, GLint tex_coord_location = -1)
      : Mesh(vertices, std::vector<float>(), std::vector<float>(), indices, mode, position_location, normal_location,
             tex_coord_location) {}

  Mesh(std::vector<float> vertices, std::vector<float> normals, std::vector<uint32_t> indices,
       GLenum mode = GL_TRIANGLES, GLint position_location = -1, GLint normal_location = -1,
       GLint tex_coord_location = -1)
      : Mesh(vertices, normals, std::vector<float>(), indices, mode, position_location, normal_location,
             tex_coord_location) {}

  Mesh(std::vector<float> vertices, std::vector<float> normals, std::vector<float> tex_coords,
       std::vector<uint32_t> indices, GLenum mode = GL_TRIANGLES, GLint position_location = -1,
       GLint normal_location = -1, GLint tex_coord_location = -1);

  Mesh(const Mesh &other);

  void create_vao(GLint position_location = -1, GLint normal_location = -1, GLint tex_coord_location = -1);
  GLuint get_vao_id() { return this->vao_id; }
  void bind_vao() { glBindVertexArray(this->vao_id); }

  void draw();

  static Mesh from_interleaved(std::vector<float> interleaved_vertices, std::vector<uint32_t> indices,
                               GLenum mode = GL_TRIANGLES, GLint position_location = -1, GLint normal_location = -1,
                               GLint tex_coord_location = -1);

  static std::vector<Mesh> from_file(const std::string &file_name, GLint position_location = -1,
                                     GLint normal_location = -1, GLint tex_coord_location = -1);

  static Mesh cube(GLint position_location = -1, GLint normal_location = -1, GLint tex_coord_location = -1);
  static Mesh sphere(GLint position_location = -1, GLint normal_location = -1, GLint tex_coord_location = -1);
  static Mesh teapot(GLint position_location = -1, GLint normal_location = -1, GLint tex_coord_location = -1);

  ~Mesh();

private:
  GLuint vao_id = 0;

  size_t vertices_count = 0;
  GLuint vertices_buffer_id = 0;
  GLuint normals_buffer_id = 0;
  GLuint tex_coords_buffer_id = 0;

  size_t indices_count = 0;
  GLuint indices_buffer_id = 0;

  GLint position_location = -1;
  GLint normal_location = -1;
  GLint tex_coord_location = -1;

  GLenum mode = GL_TRIANGLES;
};
