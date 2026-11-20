#pragma once

#include <cstddef>
#include "ShaderProgram.h"
#include "Camera.h"

class LightController {
public:
    void update(size_t sceneIndex, ShaderProgram* nightShader, Camera* camera);
};