#include "ScenePicker.h"
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Translation.h"
#include "Rotation.h"
#include "Scale.h"
#include "CompositeTransformation.h"

static float randFloat(float minVal, float maxVal) {
    return minVal + static_cast<float>(std::rand()) / (static_cast<float>(RAND_MAX) / (maxVal - minVal));
}

void ScenePicker::onMouseClick(double x, double y, bool ctrlPressed) {
    hasClick = true;
    placeStaticOnce = ctrlPressed;
    clickX = x;
    clickY = y;
}

void ScenePicker::update(
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
) {
    if (!hasClick || currentScene == nullptr) return;
    hasClick = false;

    int currentMode = placeStaticOnce ? 1 : mode;
    placeStaticOnce = false;

    int winW, winH, fbW, fbH;
    glfwGetWindowSize(window, &winW, &winH);
    glfwGetFramebufferSize(window, &fbW, &fbH);
    if (winW <= 0 || winH <= 0) return;

    GLint x = static_cast<GLint>(clickX * fbW / winW);
    GLint y = fbH - static_cast<GLint>(clickY * fbH / winH);

    GLfloat depth = 1.0f;
    GLuint stencilID = 0;
    glReadPixels(x, y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
    glReadPixels(x, y, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_INT, &stencilID);

    if (currentMode == 0) {
        std::cout << "Clicked pixel (" << x << ", " << y << "), depth: " << depth << ", stencil ID: " << stencilID << std::endl;

        if (stencilID != 0 && movingController.removeByObjectID(sceneIndex, stencilID)) {
            currentScene->removeObjectByID(stencilID);
            std::cout << "Deleted moving object ID: " << stencilID << std::endl;
            return;
        }
        currentScene->selectByID(stencilID);
        DrawableObject* selected = currentScene->getSelectedObject();
        if (selected != nullptr) {
            std::cout << "Selected object ID: " << selected->getID() << std::endl;
        }
        else {
            std::cout << "No object selected (background clicked)." << std::endl;
        }
        return;
    }

    if (sceneIndex != 2 && sceneIndex != 7 && sceneIndex != 8) return;
    if (depth >= 1.0f || camera == nullptr) return;

    glm::vec4 viewport(0.0f, 0.0f, static_cast<float>(fbW), static_cast<float>(fbH));
    glm::vec3 pos = glm::unProject(glm::vec3(x, y, depth), camera->getViewMatrix(), camera->getProjectionMatrix(), viewport);

    if (currentMode == 1) {
        auto tr = std::make_shared<CompositeTransformation>();
        DrawableObject* obj = nullptr;

        if (sceneIndex == 2 || sceneIndex == 7) {
            if (toggleModel) {
                tr->add(std::make_shared<Translation>(pos));
                tr->add(std::make_shared<Rotation>(glm::radians(randFloat(0.0f, 360.0f)), glm::vec3(0.0f, 1.0f, 0.0f)));
                tr->add(std::make_shared<Scale>(randFloat(0.28f, 0.45f)));
                obj = new DrawableObject(models[3], nightShader, tr, glm::vec3(0.15f, 0.55f, 0.15f));
            }
            else {
                tr->add(std::make_shared<Translation>(pos.x, pos.y + 0.07f, pos.z));
                tr->add(std::make_shared<Scale>(randFloat(0.16f, 0.28f)));
                obj = new DrawableObject(models[4], nightShader, tr, glm::vec3(0.15f, 0.65f, 0.18f));
            }
        }
        else if (sceneIndex == 8) {
            tr->add(std::make_shared<Translation>(pos));
            tr->add(std::make_shared<Rotation>(glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
            tr->add(std::make_shared<Scale>(0.3f));
            obj = toggleModel
                ? new DrawableObject(models[21], textureShader, tr, glm::vec3(1.0f), ocTex)
                : new DrawableObject(models[20], textureShader, tr, glm::vec3(1.0f), tableTex);
        }

        if (obj != nullptr) {
            currentScene->addObject(obj);
            toggleModel = !toggleModel;
            std::cout << "Placed static object ID: " << obj->getID()
                << " at [" << pos.x << ", " << pos.y << ", " << pos.z << "]" << std::endl;
        }
        return;
    }

    if (!movingController.hasPendingPointA()) {
        movingController.setPointA(pos);
        std::cout << "Point A set. Click point B..." << std::endl;
        return;
    }

    glm::vec3 pointA = movingController.getPointA();
    movingController.clearPendingPointA();

    auto moveTr = std::make_shared<Translation>(pointA);
    auto tr = std::make_shared<CompositeTransformation>();
    tr->add(moveTr);

    DrawableObject* movingObj = nullptr;
    float speed = 1.8f;

    if (sceneIndex == 8) {
        tr->add(std::make_shared<Rotation>(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
        tr->add(std::make_shared<Scale>(0.25f));
        movingObj = new DrawableObject(models[13], textureShader, tr, glm::vec3(1.0f), shrekTex);
        speed = 0.65f;
    }
    else {
        tr->add(std::make_shared<Scale>(0.42f));
        movingObj = new DrawableObject(models[5], shaders[2], tr, glm::vec3(1.0f, 0.35f, 0.15f));
    }

    currentScene->addObject(movingObj);
    movingController.addMovingObject({ sceneIndex, movingObj->getID(), moveTr, pointA, pos, 0.0f, speed, 1 });
    std::cout << "Spawned moving object ID: " << movingObj->getID() << std::endl;
}