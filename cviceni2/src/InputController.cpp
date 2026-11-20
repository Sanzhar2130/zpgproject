#include "InputController.h"
#include <iostream>
#include <glm/glm.hpp>

void InputController::processContinuousInput(GLFWwindow* window, Camera* camera, float deltaTime) {
    if (!camera) return;
    float speed = 3.0f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) camera->moveForward(speed);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) camera->moveBackward(speed);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) camera->moveLeft(speed);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) camera->moveRight(speed);
}

void InputController::onMouseButton(GLFWwindow* window, int button, int action, int mods, ScenePicker& picker) {
    if (button != GLFW_MOUSE_BUTTON_RIGHT && button != GLFW_MOUSE_BUTTON_LEFT) return;

    if (action == GLFW_PRESS) {
        isMousePressed = true;
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        lastX = xpos;
        lastY = ypos;

        if (button == GLFW_MOUSE_BUTTON_LEFT) {
            bool isCtrlPressed = ((mods & GLFW_MOD_CONTROL) != 0) ||
                (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) ||
                (glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS);
            picker.onMouseClick(xpos, ypos, isCtrlPressed);
        }
    }
    else if (action == GLFW_RELEASE) {
        isMousePressed = false;
    }
}

void InputController::onMouseMove(double xpos, double ypos, Camera* camera) {
    if (!isMousePressed) {
        lastX = xpos;
        lastY = ypos;
        return;
    }

    float xoffset = static_cast<float>(xpos - lastX);
    float yoffset = static_cast<float>(lastY - ypos);
    lastX = xpos;
    lastY = ypos;

    if (camera) camera->handleMouseMovement(xoffset, yoffset);
}

void InputController::onKeyboard(
    GLFWwindow* window,
    int key,
    int action,
    Camera* camera,
    const std::vector<Scene*>& scenes,
    size_t& activeSceneIndex,
    ScenePicker& picker,
    MovingObjectController& movingController
) {
    if (action != GLFW_PRESS) return;

    if (key == GLFW_KEY_ESCAPE) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
    else if (key >= GLFW_KEY_1 && key <= GLFW_KEY_9) {
        size_t idx = static_cast<size_t>(key - GLFW_KEY_1);
        if (camera && idx < scenes.size() && scenes[idx] != nullptr) {
            activeSceneIndex = idx;
            std::cout << "Switched to scene: " << scenes[idx]->getName() << std::endl;

            glm::vec3 pos(0.0f, 0.0f, 2.5f);
            if (idx == 5) pos = glm::vec3(0.0f, 0.0f, 2.35f);
            else if (idx == 2) pos = glm::vec3(0.0f, 1.5f, 5.0f);
            else if (idx == 4) pos = glm::vec3(0.0f, 5.0f, 15.0f);

            camera->setPose(pos, glm::radians(90.0f), glm::radians(-90.0f));
        }
    }
    else if (key == GLFW_KEY_P && camera) {
        camera->setProjectionType(ProjectionType::Perspective);
        std::cout << "Change projection: Perspective" << std::endl;
    }
    else if (key == GLFW_KEY_O && camera) {
        camera->setProjectionType(ProjectionType::Orthogonal);
        std::cout << "Change projection: Orthogonal" << std::endl;
    }
    else if (key == GLFW_KEY_I) {
        picker.setMode(0);
        movingController.clearPendingPointA();
        std::cout << "Mode: Select / Delete" << std::endl;
    }
    else if (key == GLFW_KEY_B) {
        picker.setMode(1);
        movingController.clearPendingPointA();
        std::cout << "Mode: Place static object" << std::endl;
    }
    else if (key == GLFW_KEY_M) {
        picker.setMode(2);
        movingController.clearPendingPointA();
        std::cout << "Mode: Place moving model" << std::endl;
    }
}