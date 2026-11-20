/**
 * @file MovingObjectController.cpp
 * @author Student (login: ZHA0067)
 * @brief Implementation of MovingObjectController.
 */
#include "MovingObjectController.h"
#include <glm/glm.hpp>

void MovingObjectController::resetLineCreation() {
    hasFirstPoint = false;
    pointA = glm::vec3(0.0f);
}

void MovingObjectController::setPointA(const glm::vec3& pos) {
    pointA = pos;
    hasFirstPoint = true;
}

void MovingObjectController::addMovingObject(const MovingObject& obj) {
    objects.push_back(obj);
}

bool MovingObjectController::removeByObjectID(size_t sceneIndex, GLuint objectID) {
    for (auto it = objects.begin(); it != objects.end(); ++it) {
        if (it->sceneIndex == sceneIndex && it->objectID == objectID) {
            objects.erase(it);
            return true;
        }
    }
    return false;
}

void MovingObjectController::clear() {
    objects.clear();
    resetLineCreation();
}

void MovingObjectController::updateSingleObject(MovingObject& mObj, float deltaTime) const {
    if (!mObj.translation) return;

    float dist = glm::distance(mObj.pointA, mObj.pointB);
    float step = (dist > 0.0001f) ? (mObj.speed * deltaTime / dist) : 0.0f;
    mObj.t += static_cast<float>(mObj.direction) * step;

    if (mObj.t >= 1.0f) {
        mObj.t = 1.0f;
        mObj.direction = -1;
    }
    else if (mObj.t <= 0.0f) {
        mObj.t = 0.0f;
        mObj.direction = 1;
    }

    glm::vec3 pos = glm::mix(mObj.pointA, mObj.pointB, mObj.t);
    mObj.translation->setPosition(pos);
}

void MovingObjectController::update(size_t activeSceneIndex, float deltaTime) {
    for (MovingObject& mObj : objects) {
        if (mObj.sceneIndex == activeSceneIndex) {
            updateSingleObject(mObj, deltaTime);
        }
    }
}