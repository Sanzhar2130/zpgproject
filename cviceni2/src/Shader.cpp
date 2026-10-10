#include "Shader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>

std::string Shader::loadFile(const char* filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Chyba: Nelze otevrit soubor se shaderem: " << filePath << std::endl;
        exit(EXIT_FAILURE);
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void Shader::checkCompileErrors(GLuint shader) {
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar infoLog[1024];
        glGetShaderInfoLog(shader, sizeof(infoLog), nullptr, infoLog);
        std::cerr << "Kompilace shaderu selhala ("
            << (shaderType == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT")
            << "):\n" << infoLog << std::endl;
        exit(EXIT_FAILURE);
    }
}

Shader::Shader(GLenum type, const char* filePath) : shaderType(type), shaderId(0) {
    std::string code = loadFile(filePath);
    const char* src = code.c_str();

    shaderId = glCreateShader(type);
    glShaderSource(shaderId, 1, &src, nullptr);
    glCompileShader(shaderId);
    checkCompileErrors(shaderId);
}

GLuint Shader::getId() const { return shaderId; }
GLenum Shader::getType() const { return shaderType; }

Shader::~Shader() {
    if (shaderId != 0) {
        glDeleteShader(shaderId);
    }
}