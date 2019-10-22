#pragma once

#include <string>

#include <GL/glew.h>

#include <linmath.h>

// TODO: error checking

class GLSLProgram {
public:
    GLSLProgram();
    ~GLSLProgram();

    static std::string readShader(const char *filepath);
    ///
    /// \brief Adds a single type of shader to the program.  Adding at least Vertex and Fragment shader is required.
    ///
    void addShader(GLuint shaderType, const char *source);
    ///
    /// \brief Links the program.  Call this after all shaders are added and frag outputs are set
    ///
    void linkProgram();

    void bind();
    void unbind();

    void setMat4(GLuint location, const mat4x4 &mat);

    ///
    /// \brief Get the location of the uniform by name
    ///
    GLuint getUniformLocation(const char *uniformName);
    ///
    /// \brief Get the location of the attribute by name
    ///
    GLuint getAttributeLocation(const char *attributeName);

private:
    GLuint shaderProgram;
};
