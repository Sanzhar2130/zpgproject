#pragma once

#include <glad/gl.h>
#include <string>

class Shader {
private:
    GLuint shaderId;
    GLenum shaderType;

    std::string loadFile(const char* filePath);
    void checkCompileErrors(GLuint shader);

public:
    Shader(GLenum type, const char* filePath);
    ~Shader();

    GLuint getId() const;
    GLenum getType() const;
};