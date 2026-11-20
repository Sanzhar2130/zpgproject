#pragma once

#include <vector>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include "Camera.h"
#include "Scene.h"
#include "ScenePicker.h"
#include "MovingObjectController.h"

class InputController {
private:
    double lastX = 512.0;
    double lastY = 384.0;
    bool isMousePressed = false;

public:
    void processContinuousInput(GLFWwindow* window, Camera* camera, float deltaTime);
    void onMouseButton(GLFWwindow* window, int button, int action, int mods, ScenePicker& picker);
    void onMouseMove(double xpos, double ypos, Camera* camera);
    void onKeyboard(
        GLFWwindow* window,
        int key,
        int action,
        Camera* camera,
        const std::vector<Scene*>& scenes,
        size_t& activeSceneIndex,
        ScenePicker& picker,
        MovingObjectController& movingController
    );
};