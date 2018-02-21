#include "mesh.hpp"

#include "cube.inl"
#include "sphere.inl"
#include "teapot.inl"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

Mesh::Mesh(const std::string &file_name, GLint position_location, GLint normal_location, GLint tex_coord_location) {
  *this = Mesh::from_file(file_name, position_location, normal_location, tex_coord_location)[0];
}

Mesh::Mesh(std::vector<float> vertices, std::vector<float> normals, std::vector<float> tex_coords,
           std::vector<uint32_t> indices, GLenum mode, GLint position_location, GLint normal_location,
           GLint tex_coord_location) {
  vertices_count = vertices.size() / 3;

  // Create buffer for vertices
  glGenBuffers(1, &this->vertices_buffer_id);
  glBindBuffer(GL_ARRAY_BUFFER, this->vertices_buffer_id);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Create buffer for normals
  if (!normals.empty()) {
    glGenBuffers(1, &this->normals_buffer_id);
    glBindBuffer(GL_ARRAY_BUFFER, this->normals_buffer_id);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), normals.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }

  // Create buffer for texture coordinates
  if (!tex_coords.empty()) {
    glGenBuffers(1, &this->tex_coords_buffer_id);
    glBindBuffer(GL_ARRAY_BUFFER, this->tex_coords_buffer_id);
    glBufferData(GL_ARRAY_BUFFER, tex_coords.size() * sizeof(float), tex_coords.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }

  if (!indices.empty()) {
    // If indices buffer is not empty => create buffer for indices
    glGenBuffers(1, &this->indices_buffer_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indices_buffer_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    this->indices_count = indices.size();
  }

  this->mode = mode;

  this->create_vao(position_location, normal_location, tex_coord_location);
}

Mesh::Mesh(const Mesh &other) {
  this->vertices_count = other.vertices_count;
  this->indices_count = other.indices_count;
  this->mode = other.mode;

  // Copy vertices
  if (other.vertices_buffer_id != 0) {
    glGenBuffers(1, &this->vertices_buffer_id);
    glBindBuffer(GL_ARRAY_BUFFER, this->vertices_buffer_id);
    glBufferData(GL_ARRAY_BUFFER, vertices_count * sizeof(float) * 3, nullptr, GL_STATIC_DRAW);

    glBindBuffer(GL_COPY_READ_BUFFER, other.vertices_buffer_id);
    glBindBuffer(GL_COPY_WRITE_BUFFER, this->vertices_buffer_id);
    glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, vertices_count * sizeof(float) * 3);
  }

  // Copy normals
  if (other.normals_buffer_id != 0) {
    glGenBuffers(1, &this->normals_buffer_id);
    glBindBuffer(GL_ARRAY_BUFFER, this->normals_buffer_id);
    glBufferData(GL_ARRAY_BUFFER, vertices_count * sizeof(float) * 3, nullptr, GL_STATIC_DRAW);

    glBindBuffer(GL_COPY_READ_BUFFER, other.normals_buffer_id);
    glBindBuffer(GL_COPY_WRITE_BUFFER, this->normals_buffer_id);
    glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, vertices_count * sizeof(float) * 3);
  }

  // Copy texture coordinates
  if (other.tex_coords_buffer_id != 0) {
    glGenBuffers(1, &this->tex_coords_buffer_id);
    glBindBuffer(GL_ARRAY_BUFFER, this->tex_coords_buffer_id);
    glBufferData(GL_ARRAY_BUFFER, vertices_count * sizeof(float) * 2, nullptr, GL_STATIC_DRAW);

    glBindBuffer(GL_COPY_READ_BUFFER, other.tex_coords_buffer_id);
    glBindBuffer(GL_COPY_WRITE_BUFFER, this->tex_coords_buffer_id);
    glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, vertices_count * sizeof(float) * 2);
  }

  // Copy indices buffer
  if (other.indices_buffer_id != 0) {
    glGenBuffers(1, &this->indices_buffer_id);
    glBindBuffer(GL_ARRAY_BUFFER, this->indices_buffer_id);
    glBufferData(GL_ARRAY_BUFFER, indices_count * sizeof(uint32_t), nullptr, GL_STATIC_DRAW);

    glBindBuffer(GL_COPY_READ_BUFFER, other.indices_buffer_id);
    glBindBuffer(GL_COPY_WRITE_BUFFER, this->indices_buffer_id);
    glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, indices_count * sizeof(uint32_t));
  }

  // Set up VAO if the other object has one too
  if (other.vao_id != 0) {
    this->create_vao(other.position_location, other.normal_location, other.tex_coord_location);
  }

  glBindBuffer(GL_COPY_READ_BUFFER, 0);
  glBindBuffer(GL_COPY_WRITE_BUFFER, 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Mesh::create_vao(GLint position_location, GLint normal_location, GLint tex_coord_location) {
  // In case we want recreate it
  glDeleteVertexArrays(1, &this->vao_id);

  // Create a vertex array object for the geometry
  glGenVertexArrays(1, &this->vao_id);

  // Set the parameters of the geometry
  glBindVertexArray(this->vao_id);

  if (position_location >= 0) {
    this->position_location = position_location;
    glBindBuffer(GL_ARRAY_BUFFER, this->vertices_buffer_id);
    glEnableVertexAttribArray(position_location);
    glVertexAttribPointer(position_location, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
  }
  if (normal_location >= 0) {
    this->normal_location = normal_location;
    glBindBuffer(GL_ARRAY_BUFFER, this->normals_buffer_id);
    glEnableVertexAttribArray(normal_location);
    glVertexAttribPointer(normal_location, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
  }
  if (tex_coord_location >= 0) {
    this->tex_coord_location = tex_coord_location;
    glBindBuffer(GL_ARRAY_BUFFER, this->tex_coords_buffer_id);
    glEnableVertexAttribArray(tex_coord_location);
    glVertexAttribPointer(tex_coord_location, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
  }

  if (this->indices_buffer_id != 0) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indices_buffer_id);
  }

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::draw() {
  this->bind_vao();

  if (this->indices_buffer_id > 0) {
    glDrawElements(this->mode, static_cast<GLsizei>(this->indices_count), GL_UNSIGNED_INT, nullptr);
  } else {
    glDrawArrays(this->mode, 0, static_cast<GLsizei>(this->vertices_count));
  }
}

Mesh Mesh::from_interleaved(std::vector<float> interleaved_vertices, std::vector<uint32_t> indices, GLenum mode,
                            GLint position_location, GLint normal_location, GLint tex_coord_location) {
  // Deinterleave and then call deinterleaved constructor
  std::vector<float> vertices;
  std::vector<float> normals;
  std::vector<float> tex_coords;

  for (size_t vertex_offset = 0; vertex_offset < interleaved_vertices.size(); vertex_offset += 8) {
    for (int i = 0; i < 3; i++) {
      vertices.push_back(interleaved_vertices[vertex_offset + i]);
      normals.push_back(interleaved_vertices[vertex_offset + 3 + i]);
    }

    tex_coords.push_back(interleaved_vertices[vertex_offset + 6]);
    tex_coords.push_back(interleaved_vertices[vertex_offset + 7]);
  }

  Mesh mesh(vertices, normals, tex_coords, indices, mode);
  mesh.create_vao(position_location, normal_location, tex_coord_location);

  return mesh;
}

Mesh Mesh::cube(GLint position_location, GLint normal_location, GLint tex_coord_location) {
  return Mesh::from_interleaved(std::vector<float>(cube_vertices, std::end(cube_vertices)),
                                std::vector<uint32_t>(cube_indices, std::end(cube_indices)), GL_TRIANGLES,
                                position_location, normal_location, tex_coord_location);
}

Mesh Mesh::sphere(GLint position_location, GLint normal_location, GLint tex_coord_location) {
  return Mesh::from_interleaved(std::vector<float>(sphere_vertices, std::end(sphere_vertices)),
                                std::vector<uint32_t>(sphere_indices, std::end(sphere_indices)), GL_TRIANGLE_STRIP,
                                position_location, normal_location, tex_coord_location);
}

Mesh Mesh::teapot(GLint position_location, GLint normal_location, GLint tex_coord_location) {
  return Mesh::from_interleaved(std::vector<float>(teapot_vertices, std::end(teapot_vertices)),
                                std::vector<uint32_t>(teapot_indices, std::end(teapot_indices)), GL_TRIANGLE_STRIP,
                                position_location, normal_location, tex_coord_location);
}

std::vector<Mesh> Mesh::from_file(const std::string &file_name, GLint position_location, GLint normal_location,
                                  GLint tex_coord_location) {
  tinyobj::attrib_t attrib;
  std::vector<tinyobj::shape_t> shapes;
  std::vector<tinyobj::material_t> materials;

  std::string err;
  bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, file_name.c_str());

  if (!ret) {
    throw "Could not load object file.";
  }

  std::vector<Mesh> meshes;

  // Loop over shapes
  for (size_t s = 0; s < shapes.size(); s++) {
    std::vector<float> vertices;
    std::vector<float> normals;
    std::vector<float> tex_coords;

    for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
      // Loop over vertices in the face/polygon
      // (because of triangulate true in LoadObj number of vertices
      // per face/polygon is always 3
      for (size_t v = 0; v < 3; v++) {
        // access to vertex
        tinyobj::index_t idx = shapes[s].mesh.indices[f * 3 + v];

        for (int i = 0; i < 3; i++) {
          vertices.push_back(attrib.vertices[3 * idx.vertex_index + i]);

          if (!attrib.normals.empty()) {
            vertices.push_back(attrib.normals[3 * idx.normal_index + i]);
          }
        }

        if (!attrib.texcoords.empty()) {

          tex_coords.push_back(attrib.texcoords[2 * idx.texcoord_index + 0]);
          tex_coords.push_back(attrib.texcoords[2 * idx.texcoord_index + 1]);
        }
      }
    }

    meshes.push_back(Mesh(vertices, normals, tex_coords, std::vector<uint32_t>()));
    meshes[meshes.size() - 1].create_vao(position_location, normal_location, tex_coord_location);
  }

  return meshes;
}

Mesh::~Mesh() {
  // Deallocate all resources on GPU
  // Everything can be deleted without checking because we initialize it
  // to 0 and specification ignores 0 in glDelete* calls
  glDeleteVertexArrays(1, &this->vao_id);

  glDeleteBuffers(1, &this->vertices_buffer_id);
  glDeleteBuffers(1, &this->normals_buffer_id);
  glDeleteBuffers(1, &this->tex_coords_buffer_id);
  glDeleteBuffers(1, &this->indices_buffer_id);
}
