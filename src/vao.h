#pragma once

#include <GL/glew.h>

class VBO {
public:
    VBO();
    ~VBO();

    void setData(int length, const void *data, GLuint usageHint = GL_STATIC_DRAW);
    void bind();
    void unbind();

private:
    GLuint vbo;
};

struct VAOAttribute {
    VAOAttribute(GLuint location, GLenum type, GLint size, GLuint divisor = 0);

    ///
    /// \brief The ID representing this attribute in the shader
    ///
    GLuint location;
    ///
    /// \brief the type of the attribute (GL_FLOAT, GL_DOUBLE, GL_INT, etc..)
    ///
    GLenum type;
    ///
    /// \brief The number of components (eg 4 means a GLSL vec4). Either 1, 2, 3, or 4
    ///
    GLint size; //
    ///
    /// \brief The GL instancing divisor
    ///
    GLuint divisor;

    static VAOAttribute END;

    bool operator ==(const VAOAttribute &rhs) const;
    bool operator !=(const VAOAttribute &rhs) const;

    ///
    /// \brief Returns the number of bytes needed to store a single value/element of the attribute
    ///
    int bytesRequired() const;

    ///
    /// \brief Returns the basic type of the input GL type.
    /// For use in determining which version of glVertexAttribPointer to use
    ///
    static GLenum basicType(GLenum type);
};

class VAO {
public:
    VAO();
    ~VAO();

    ///
    /// \brief Sets the attribute pointer given the currently bound VBO
    ///
    void setAttributePointer(const VAOAttribute &attribute);
    ///
    /// \brief Sets attribute pointers given the currently bound VBO
    ///
    void setAttributePointers(const VAOAttribute *attributes);

    void bind();
    void unbind();

private:
    void setAttributePointer(const VAOAttribute &attribute, long offset, long stride);

    GLuint vao;
};
