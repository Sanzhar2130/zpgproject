/**
 * @file Camera.cpp
 * @author Student (login: ZHA0067)
 * @brief Implementation of Camera functionality.
 */
#include "Camera.h"
#include <algorithm>

Camera::Camera(glm::vec3 startEye, float startAlpha, float startFi, float screenAspect)
    : eye(startEye),
    alpha(startAlpha),
    fi(startFi),
    up(glm::vec3(0.0f, 1.0f, 0.0f)),
    fov(glm::radians(60.0f)),
    aspect(screenAspect),
    zNear(0.1f),
    zFar(100.0f),
    projType(ProjectionType::Perspective)
{
    updateVectors();
    updateProjection();
}

void Camera::updateVectors() {
    // Calculation of the view direction vector
    target.x = sin(alpha) * cos(fi);
    target.z = sin(alpha) * sin(fi);
    target.y = cos(alpha);

    target = glm::normalize(target);
    updateView();
}

void Camera::updateView() {
    // Formation of the view matrix: LookAt(eye, where we look, where up)
    viewMatrix = glm::lookAt(eye, eye + target, up);
    notify();
}

void Camera::updateProjection() {
    if (projType == ProjectionType::Perspective) {
        // zNear is strictly 0.1f (not 0.0f to eliminate division by zero in the Z-buffer)
        projectionMatrix = glm::perspective(fov, aspect, zNear, zFar);
    }
    else {
        float orthoSize = 5.0f;
        projectionMatrix = glm::ortho(-orthoSize * aspect, orthoSize * aspect, -orthoSize, orthoSize, zNear, zFar);
    }
    notify();
}

void Camera::attach(Observer* observer) {
    if (observer) {
        observers.push_back(observer);
        // Immediately pass the current state to the new observer
        observer->update(viewMatrix, projectionMatrix);
    }
}

void Camera::detach(Observer* observer) {
    observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
}

void Camera::notify() {
    for (Observer* obs : observers) {
        if (obs) {
            obs->update(viewMatrix, projectionMatrix);
        }
    }
}

void Camera::handleMouseMovement(float xOffset, float yOffset, float sensitivity) {
    fi += xOffset * sensitivity;
    alpha -= yOffset * sensitivity;

    // Gimbal Lock Protection: limit alpha zenith angle
    const float eps = 0.01f;
    if (alpha < eps) alpha = eps;
    if (alpha > glm::pi<float>() - eps) alpha = glm::pi<float>() - eps;

    updateVectors();
}

void Camera::moveForward(float delta) {
    eye += target * delta;
    updateView();
}

void Camera::moveBackward(float delta) {
    eye -= target * delta;
    updateView();
}

void Camera::moveLeft(float delta) {
    glm::vec3 right = glm::normalize(glm::cross(target, up));
    eye -= right * delta;
    updateView();
}

void Camera::moveRight(float delta) {
    glm::vec3 right = glm::normalize(glm::cross(target, up));
    eye += right * delta;
    updateView();
}

void Camera::setAspect(float newAspect) {
    if (newAspect > 0.0f) {
        aspect = newAspect;
        updateProjection();
    }
}

void Camera::setProjectionType(ProjectionType type) {
    projType = type;
    updateProjection();
}

glm::mat4 Camera::getViewMatrix() const { return viewMatrix; }
glm::mat4 Camera::getProjectionMatrix() const { return projectionMatrix; }
glm::vec3 Camera::getPosition() const { return eye; }