#include "texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>


GLTexture::GLTexture() {
    glGenTextures(1, &texture);
}

GLTexture::~GLTexture() {
    glDeleteTextures(1, &texture);
}

void GLTexture::loadFromFile(const std::string path) {
    int width, height, channels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &channels, 0);
    bind();
    GLenum channelType = 0;
    switch (channels) {
    case 1:
        channelType = GL_RED;
        break;
    case 2:
        channelType = GL_RG;
        break;
    case 3:
        channelType = GL_RGB;
        break;
    case 4:
        channelType = GL_RGBA;
    }
    glTexImage2D(GL_TEXTURE_2D, 0, channelType, width, height, 0, channelType, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    unbind();
    stbi_image_free(data);
}

void GLTexture::bind() {
    glBindTexture(GL_TEXTURE_2D, texture);
}

void GLTexture::unbind() {
    glBindTexture(GL_TEXTURE_2D, 0);
}
