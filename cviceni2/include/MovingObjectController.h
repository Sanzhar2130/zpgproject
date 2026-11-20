/**
 * @file MovingObjectController.h
 * @author Student (login: ZHA0067)
 * @brief Controller managing linear ping-pong movement of scene objects.
 */
#pragma once

#include <vector>
#include <memory>
#include <glad/gl.h>
#include <glm/glm.hpp>
#include "Translation.h"

struct MovingObject {
    size_t sceneIndex;
    GLuint objectID;
    std::shared_ptr<Translation> translation;
    glm::vec3 pointA;
    glm::vec3 pointB;
    float t = 0.0f;
    float speed = 1.5f;
    int direction = 1;
};

class MovingObjectController {
private:
    std::vector<MovingObject> objects;
    bool hasFirstPoint = false;
    glm::vec3 pointA = glm::vec3(0.0f);

    void updateSingleObject(MovingObject& mObj, float deltaTime) const;

public:
    void resetLineCreation();
    bool hasPendingPointA() const { return hasFirstPoint; }
    const glm::vec3& getPointA() const { return pointA; }
    void setPointA(const glm::vec3& pos);
    void clearPendingPointA() { hasFirstPoint = false; }

    void addMovingObject(const MovingObject& obj);
    bool removeByObjectID(size_t sceneIndex, GLuint objectID);
    void clear();

    void update(size_t activeSceneIndex, float deltaTime);
};