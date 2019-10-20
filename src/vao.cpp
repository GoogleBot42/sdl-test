#include "vao.h"

// VBO

VBO::VBO() {
    glGenBuffers(1, &vbo);
}

VBO::~VBO() {
    glDeleteBuffers(1, &vbo);
}

void VBO::setData(int length, const void *data, GLuint usageHint) {
    bind();
    glBufferData(GL_ARRAY_BUFFER, length, data, GL_STATIC_DRAW);
    unbind();
}

void VBO::bind() {
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
}

void VBO::unbind() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// VAOAttribute

VAOAttribute::VAOAttribute(GLuint location, GLenum type, GLint size, GLuint divisor)
    : location(location), type(type), size(size), divisor(divisor) {}

VAOAttribute VAOAttribute::END = VAOAttribute(0, 0, 0);

bool VAOAttribute::operator ==(const VAOAttribute &rhs) const {
    return location == rhs.location &&
            type == rhs.type &&
            size == rhs.size;
}

bool VAOAttribute::operator !=(const VAOAttribute &rhs) const {
    return !(*this == rhs);
}

int VAOAttribute::bytesRequired() const {
    int typeSize = 0;
    switch (type) {
    case GL_BYTE:
    case GL_UNSIGNED_BYTE:
        typeSize = 1;
        break;
    case GL_SHORT:
    case GL_UNSIGNED_SHORT:
        typeSize = 2;
        break;
    case GL_INT:
    case GL_UNSIGNED_INT:
        typeSize = 4;
        break;
    case GL_HALF_FLOAT:
        typeSize = 2;
        break;
    case GL_FLOAT:
        typeSize = 4;
        break;
    case GL_DOUBLE:
        typeSize = 8;
        break;
    case GL_FIXED:
        typeSize = 4;
        break;
    case GL_INT_2_10_10_10_REV:
    case GL_UNSIGNED_INT_2_10_10_10_REV:
    case GL_UNSIGNED_INT_10F_11F_11F_REV:
        return 4; // special case
    }
    return typeSize * size;
}

GLenum VAOAttribute::basicType(GLenum type) {
    switch (type) {
    case GL_BYTE:
    case GL_UNSIGNED_BYTE:
    case GL_SHORT:
    case GL_UNSIGNED_SHORT:
    case GL_INT:
    case GL_UNSIGNED_INT:
        return GL_INT;
    case GL_HALF_FLOAT:
    case GL_FLOAT:
    case GL_FIXED:
    case GL_INT_2_10_10_10_REV:
    case GL_UNSIGNED_INT_2_10_10_10_REV:
    case GL_UNSIGNED_INT_10F_11F_11F_REV:
        return GL_FLOAT;
    case GL_DOUBLE:
        return GL_DOUBLE;
    }
    return GL_FLOAT;
}

// VAO

VAO::VAO() {
    glGenVertexArrays(1, &vao);
}

VAO::~VAO() {
    glDeleteVertexArrays(1, &vao);
}

void VAO::setAttributePointer(const VAOAttribute &attribute) {
    bind();
    setAttributePointer(attribute, 0, 0);
    unbind();
}

void VAO::setAttributePointers(const VAOAttribute *attributes) {
    // first find out the length and what the stride is
    int count = 0;
    long stride = 0;
    const VAOAttribute *currentAttrib = attributes;
    while (*currentAttrib != VAOAttribute::END) {
        stride += currentAttrib->bytesRequired();
        count++;
        currentAttrib++;
    }

    bind();

    // add the attributes one at a time
    long previousCombinedSize = 0;
    for (int i=0; i<count; i++) {
        setAttributePointer(attributes[i], previousCombinedSize, stride);
        previousCombinedSize += attributes[i].bytesRequired();
    }

    unbind();
}

void VAO::bind() {
    glBindVertexArray(vao);
}

void VAO::unbind() {
    glBindVertexArray(0);
}

void VAO::setAttributePointer(const VAOAttribute &attribute, long offset, long stride) {
    const GLenum basicType = VAOAttribute::basicType(attribute.size);

    if (basicType == GL_FLOAT)
        glVertexAttribPointer(attribute.location, attribute.size, attribute.type, GL_FALSE, stride, (const void*)offset);
    else if (basicType == GL_INT)
        glVertexAttribIPointer(attribute.location, attribute.size, attribute.type, stride, (const void*)offset);
    else if (basicType == GL_DOUBLE)
        glVertexAttribLPointer(attribute.location, attribute.size, GL_DOUBLE, stride, (const void*)offset);

    glVertexAttribDivisor(attribute.location, attribute.divisor);
    glEnableVertexAttribArray(attribute.location);
}
