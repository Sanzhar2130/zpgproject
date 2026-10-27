/**
 * @file Camera.h
 * @author Student (login: ZHA0067)
 * @brief Implementation of Camera functionality.
 */
#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Observer.h"

enum class ProjectionType {
    Perspective,
    Orthogonal
};

class Camera {
private:
    // Position and orientation in space
    glm::vec3 eye;
    glm::vec3 target;
    glm::vec3 up;

    // Spherical angles (from the ZPG lecture)
    // alpha - zenith angle (from the Y-axis), fi - azimuthal angle (in the XZ-plane)
    float alpha;
    float fi;

    // Projection parameters
    float fov;
    float aspect;
    float zNear;
    float zFar;
    ProjectionType projType;

    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;

    // List of observers (Shaders)
    std::vector<Observer*> observers;

    void updateVectors();
    void updateProjection();
    void updateView();

public:
    Camera(glm::vec3 startEye = glm::vec3(0.0f, 2.0f, 5.0f),
        float startAlpha = glm::radians(90.0f),
        float startFi = glm::radians(-90.0f),
        float screenAspect = 4.0f / 3.0f);

    // Pattern Observer
    void attach(Observer* observer);
    void detach(Observer* observer);
    void notify();

    void handleMouseMovement(float xOffset, float yOffset, float sensitivity = 0.005f);

    void moveForward(float delta);
    void moveBackward(float delta);
    void moveLeft(float delta);
    void moveRight(float delta);

    // Projection setup
    void setAspect(float newAspect);
    void setProjectionType(ProjectionType type);


    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix() const;
    glm::vec3 getPosition() const;

    void setPosition(const glm::vec3& newEye) {
        eye = newEye;
        updateView();
    }

    void setOrientation(float newAlpha, float newFi) {
        alpha = newAlpha;
        fi = newFi;
        updateVectors();
    }
};