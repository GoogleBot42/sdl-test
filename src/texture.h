#pragma once

#include <string>

#include <GL/glew.h>

class GLTexture {
public:
    GLTexture();
    ~GLTexture();

    void loadFromFile(const std::string path);

    void bind();
    void unbind();

private:
    GLuint texture;
};
