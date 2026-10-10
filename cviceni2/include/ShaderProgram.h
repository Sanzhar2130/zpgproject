/**
 * @file ShaderProgram.h
 * @author Student (login: ZHA0067)
 * @brief ShaderProgram
 */
#pragma once

#include <glad/gl.h>
#include <glm/glm.hpp>
#include <string>
#include "Shader.h"

class ShaderProgram {
private:
    GLuint programId;

    void checkLinkErrors(GLuint program);
    GLint getUniformLocation(const std::string& name) const;

public:
    ShaderProgram();
    ShaderProgram(const Shader& vertexShader, const Shader& fragmentShader);
    ~ShaderProgram();

    void attachShader(const Shader& shader);
    void link();
    void use() const;

    GLuint getId() const;

    void setUniform(const std::string& name, float value) const;
    void setUniform(const std::string& name, int value) const;
    void setUniform(const std::string& name, float x, float y) const;
    void setUniform(const std::string& name, float x, float y, float z) const;
    void setUniform(const std::string& name, float x, float y, float z, float w) const;
    void setUniform(const std::string& name, const glm::vec3& vector) const;
    void setUniform(const std::string& name, const glm::mat4& matrix) const;
};