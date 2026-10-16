/**
 * @file ShaderProgram.h
 * @author Student (login: ZHA0067)
 * @brief ShaderProgram
 */
#pragma once

#include <string>
#include <glad/gl.h>
#include <glm/glm.hpp>
#include "Shader.h"
#include "Observer.h"

class ShaderProgram : public Observer {
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

    // Реализация интерфейса Observer: автоматическое получение матриц от Camera
    void update(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) override;

    // Методы отправки uniform-переменных
    void setUniform(const std::string& name, float value) const;
    void setUniform(const std::string& name, int value) const;
    void setUniform(const std::string& name, float x, float y) const;
    void setUniform(const std::string& name, float x, float y, float z) const;
    void setUniform(const std::string& name, float x, float y, float z, float w) const;
    void setUniform(const std::string& name, const glm::vec3& vector) const;
    void setUniform(const std::string& name, const glm::mat4& matrix) const;
};