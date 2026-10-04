#pragma once

#include <glad/gl.h>
#include <string>

class Shader {
private:
    GLuint shaderId;
    GLenum shaderType;

    std::string loadSourceFromFile(const char* filePath);
    void checkCompilationErrors(GLuint shader, const char* filePath);


public:
    Shader(GLenum type, const char* filePath);
    ~Shader();

    GLuint getId() const { return shaderId; }
};