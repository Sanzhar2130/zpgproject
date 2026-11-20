#pragma once

#include <cstddef>
#include <glad/gl.h>
#include "ShaderProgram.h"
#include "Model.h"

class SkyboxRenderer {
public:
    void render(size_t sceneIndex, ShaderProgram* skyboxShader, GLuint skyboxTex, Model* skyboxModel);
};