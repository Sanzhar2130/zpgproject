#include "Shader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>

std::string Shader::loadSourceFromFile(const char* filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Chyba: Nelze otevrit soubor se shaderem: " << filePath << std::endl;
        exit(EXIT_FAILURE);
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void Shader::checkCompilationErrors(GLuint shader, const char* filePath) {
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[1024];
        glGetShaderInfoLog(shader, sizeof(infoLog), nullptr, infoLog);
        std::cerr << "Kompilace shaderu selhala (" << filePath << "):\n" << infoLog << std::endl;
        exit(EXIT_FAILURE);
    }
}

Shader::Shader(GLenum type, const char* filePath) : shaderType(type), shaderId(0) {
    std::string code = loadSourceFromFile(filePath);
    const char* source = code.c_str();

    shaderId = glCreateShader(type);
    if (shaderId == 0) {
        std::cerr << "Nelze vytvorit shader objekt." << std::endl;
        exit(EXIT_FAILURE);
    }

    glShaderSource(shaderId, 1, &source, nullptr);
    glCompileShader(shaderId);
    checkCompilationErrors(shaderId, filePath);
}

Shader::~Shader() {
    if (shaderId != 0) {
        glDeleteShader(shaderId);
    }
}