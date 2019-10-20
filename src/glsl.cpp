#include "glsl.h"

#include <fstream>
#include <iostream>

// GLSLProgram

GLSLProgram::GLSLProgram() {
    shaderProgram = glCreateProgram();
}

GLSLProgram::~GLSLProgram() {
    glDeleteProgram(shaderProgram);
}

std::string GLSLProgram::readShader(const char *filepath) {
    // TODO: handle includes
    std::ifstream ifs(filepath);
    return std::string(std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>());
}

void GLSLProgram::addShader(GLuint shaderType, const char *source) {
    GLuint newShader = glCreateShader(shaderType);
    glShaderSource(newShader, 1, &source, NULL);
    glCompileShader(newShader);

    // check for compile error
    int compiled;
    glGetShaderiv(newShader, GL_COMPILE_STATUS, &compiled);
    if (!compiled) {
        int length;
        glGetShaderiv(newShader, GL_INFO_LOG_LENGTH, &length);
        char *msg = new char[length];
        glGetShaderInfoLog(newShader, length, NULL, msg);
        std::cerr << msg << std::endl;
        delete[] msg;
    } else {
        glAttachShader(shaderProgram, newShader);
    }
    glDeleteShader(newShader);
}

void GLSLProgram::setFragOutputLocation(GLuint colorNum, const char *attributeName) {
    glBindFragDataLocation(shaderProgram, colorNum, attributeName);
}

void GLSLProgram::linkProgram() {
    glLinkProgram(shaderProgram);

    // check for link error
    int linked;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &linked);
    if (!linked) {
        int length;
        glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &length);
        char *msg = new char[length];
        glGetProgramInfoLog(shaderProgram, length, NULL, msg);
        glDeleteProgram(shaderProgram);
        shaderProgram = 0;
        std::cerr << msg << std::endl;
        delete[] msg;
    }
}

void GLSLProgram::bind() {
    glUseProgram(shaderProgram);
}

void GLSLProgram::unbind() {
    glUseProgram(0);
}

void GLSLProgram::setMat4(GLuint location, const mat4x4 &mat) {
    glUniformMatrix4fv(location, 1, GL_FALSE, &mat[0][0]);
}

GLuint GLSLProgram::getUniformLocation(const char *uniformName) {
    return glGetUniformLocation(shaderProgram, uniformName);
}

GLuint GLSLProgram::getAttributeLocation(const char *attributeName) {
    return glGetAttribLocation(shaderProgram, attributeName);
}
