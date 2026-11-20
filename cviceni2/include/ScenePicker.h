#pragma once

#include <vector>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include "Scene.h"
#include "Model.h"
#include "ShaderProgram.h"
#include "Camera.h"
#include "MovingObjectController.h"

class ScenePicker {
private:
    int mode = 0; 
    bool hasClick = false;
    bool placeStaticOnce = false;
    bool toggleModel = true;
    double clickX = 0.0;
    double clickY = 0.0;

public:
    void setMode(int newMode) { mode = newMode; }
    void onMouseClick(double x, double y, bool ctrlPressed);

    void update(
        GLFWwindow* window,
        Camera* camera,
        Scene* currentScene,
        size_t sceneIndex,
        const std::vector<Model*>& models,
        const std::vector<ShaderProgram*>& shaders,
        ShaderProgram* nightShader,
        ShaderProgram* textureShader,
        GLuint shrekTex,
        GLuint tableTex,
        GLuint ocTex,
        MovingObjectController& movingController
    );
};