/**
 * @file ShaderFactory.h
 * @author Student (login: ZHA0067)
 * @brief Factory for creating shader programs used in the application.
 */
#pragma once

#include <vector>
#include "ShaderProgram.h"

struct ShaderBundle {
    std::vector<ShaderProgram*> allShaders;
    ShaderProgram* clockShader = nullptr;
    ShaderProgram* treeShader = nullptr;
    ShaderProgram* phongShader = nullptr;
    ShaderProgram* blinnShader = nullptr;
    ShaderProgram* ambientShader = nullptr;
    ShaderProgram* blinnColorShader = nullptr;
    ShaderProgram* nightShader = nullptr;
    ShaderProgram* textureShader = nullptr;
    ShaderProgram* skyboxShader = nullptr;
};

class ShaderFactory {
public:
    static ShaderBundle createDefaultShaders();
};