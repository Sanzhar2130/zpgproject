/**
 * @file Shader.h
 * @author Student (login: ZHA0067)
 * @brief Shader class for handling OpenGL shader programs.
 */
#pragma once

#include <glad/gl.h>
#include <string>

class Camera;

class Shader {
private:
    GLuint shaderId;
    GLenum shaderType;
	Camera* cam;

    std::string loadFile(const char* filePath);
    void checkCompileErrors(GLuint shader);

public:
    Shader(GLenum type, const char* filePath);
    ~Shader();


    GLuint getId() const;
    GLenum getType() const;
};