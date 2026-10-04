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
        char infoLog[1024];
        glGetProgramInfoLog(program, sizeof(infoLog), nullptr, infoLog);
        std::cerr << "Linkovani shader programu selhalo:\n" << infoLog << std::endl;
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


GLint ShaderProgram::getUniformLocation(const std::string& name) const {
    GLint location = glGetUniformLocation(programId, name.c_str());
    if (location == -1) {
        return -1;
    }
    return location;
}

void ShaderProgram::setUniformMatrix4fv(const std::string& name, const glm::mat4& matrix) const {
    GLint location = getUniformLocation(name);
    if (location != -1) {
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    }
}

void ShaderProgram::setUniform3f(const std::string& name, const glm::vec3& value) const {
    GLint location = getUniformLocation(name);
    if (location != -1) {
        glUniform3f(location, value.x, value.y, value.z);
    }
}

void ShaderProgram::setUniform(const std::string& name, float value) const {
    GLint location = getUniformLocation(name);
    if (location != -1) {
        glUniform1f(location, value);
    }
}

void ShaderProgram::setUniform(const std::string& name, int value) const {
    GLint location = getUniformLocation(name);
    if (location != -1) {
        glUniform1i(location, value);
    }
}

ShaderProgram::~ShaderProgram() {
    if (programId != 0) {
        glDeleteProgram(programId);
    }
}
