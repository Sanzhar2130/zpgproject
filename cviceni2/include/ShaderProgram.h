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
    GLuint getId() const { return programId; }


public:
    ShaderProgram();
    ShaderProgram(const Shader& vertexShader, const Shader& fragmentShader);
    ~ShaderProgram();

    void attachShader(const Shader& shader);
    void link();
    void use() const;


    void setUniformMatrix4fv(const std::string& name, const glm::mat4& matrix) const;
    void setUniform3f(const std::string& name, const glm::vec3& value) const;
    void setUniform(const std::string& name, float value) const;
    void setUniform(const std::string& name, int value) const;

};