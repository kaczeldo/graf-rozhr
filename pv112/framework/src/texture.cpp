#include "texture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

GLuint load_texture_2d(const std::string& filename) {
    int width, height, channels;
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &channels, STBI_rgb_alpha);

    GLuint texture_id;
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    glTexImage2D(GL_TEXTURE_2D,
                 0,                    // we are setting mipmap level 0
                 GL_RGBA,              // base internal format (inside GPU's memory)
                 width,
                 height,
                 0,                    // border - deprecated, always 0
                 GL_RGBA,              // format of the pixel data copied to GPU
                 GL_UNSIGNED_BYTE,     // type of the pixel data
                 data);                // pointer to data


    return texture_id;
}

GLuint load_texture_cubemap(std::string filenames[6]) {
    GLuint texture_id;
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id);

    for(uint8_t index = 0; index < 6; index++) {
        const std::string filename = filenames[index];
        const GLenum side = cubemap_sides[index];

        int width, height, channels;
        unsigned char *data = stbi_load(filename.c_str(), &width, &height, &channels, STBI_rgb_alpha);

        glTexImage2D(side,
                     0,                    // we are setting mipmap level 0
                     GL_RGBA,              // base internal format (inside GPU's memory)
                     width,
                     height,
                     0,                    // border - deprecated, always 0
                     GL_RGBA,              // format of the pixel data copied to GPU
                     GL_UNSIGNED_BYTE,     // type of the pixel data
                     data);                // pointer to data
    }

    return texture_id;
}
