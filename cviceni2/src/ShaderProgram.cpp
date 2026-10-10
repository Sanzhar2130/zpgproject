/**
 * @file ShaderProgram.cpp
 * @author Student (login: ZHA0067)
 * @brief Implementation of ShaderProgram class
 */
#include "ShaderProgram.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <cstdlib>

ShaderProgram::ShaderProgram() {
    programId = glCreateProgram();
}

ShaderProgram::ShaderProgram(const Shader& vertexShader, const Shader& fragmentShader) {
    programId = glCreateProgram();
    attachShader(vertexShader);
    attachShader(fragmentShader);
    link();
}

void ShaderProgram::attachShader(const Shader& shader) {
    glAttachShader(programId, shader.getId());
}

void ShaderProgram::checkLinkErrors(GLuint program) {
    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        GLchar infoLog[1024];
        glGetProgramInfoLog(program, sizeof(infoLog), nullptr, infoLog);
        std::cerr << "Linkovani shaderoveho programu selhalo:\n" << infoLog << std::endl;
        exit(EXIT_FAILURE);
    }
}

void ShaderProgram::link() {
    glLinkProgram(programId);
    checkLinkErrors(programId);
}

void ShaderProgram::use() const {
    glUseProgram(programId);
}

GLuint ShaderProgram::getId() const {
    return programId;
}

GLint ShaderProgram::getUniformLocation(const std::string& name) const {
    GLint location = glGetUniformLocation(programId, name.c_str());
    if (location == -1) {
        std::cerr << "Warning: Uniform '" << name << "' nebyl v shaderu nalezen (-1)!" << std::endl;
    }
    return location;
}

void ShaderProgram::setUniform(const std::string& name, float value) const {
    GLint loc = getUniformLocation(name);
    if (loc != -1) glUniform1f(loc, value);
}

void ShaderProgram::setUniform(const std::string& name, int value) const {
    GLint loc = getUniformLocation(name);
    if (loc != -1) glUniform1i(loc, value);
}

void ShaderProgram::setUniform(const std::string& name, float x, float y) const {
    GLint loc = getUniformLocation(name);
    if (loc != -1) glUniform2f(loc, x, y);
}

void ShaderProgram::setUniform(const std::string& name, float x, float y, float z) const {
    GLint loc = getUniformLocation(name);
    if (loc != -1) glUniform3f(loc, x, y, z);
}

void ShaderProgram::setUniform(const std::string& name, float x, float y, float z, float w) const {
    GLint loc = getUniformLocation(name);
    if (loc != -1) glUniform4f(loc, x, y, z, w);
}

void ShaderProgram::setUniform(const std::string& name, const glm::vec3& vector) const {
    GLint loc = getUniformLocation(name);
    if (loc != -1) glUniform3f(loc, vector.x, vector.y, vector.z);
}

void ShaderProgram::setUniform(const std::string& name, const glm::mat4& matrix) const {
    GLint loc = getUniformLocation(name);
    if (loc != -1) {
        glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(matrix));
    }
}

ShaderProgram::~ShaderProgram() {
    if (programId != 0) {
        glDeleteProgram(programId);
    }
}