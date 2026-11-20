/**
 * @file Camera.cpp
 * @author Student (login: ZHA0067)
 * @brief Implementation of Camera functionality.
 */
#include "Camera.h"
#include <algorithm>
#include <cmath>
#include <glm/gtc/constants.hpp>

Camera::Camera(glm::vec3 startEye, float startAlpha, float startFi, float screenAspect)
    : eye(startEye),
    alpha(startAlpha),
    fi(startFi),
    up(glm::vec3(0.0f, 1.0f, 0.0f)),
    fov(glm::radians(60.0f)),
    aspect((screenAspect > 0.0f) ? screenAspect : (4.0f / 3.0f)),
    zNear(0.1f),
    zFar(100.0f),
    orthoHalfHeight(5.0f),
    projType(ProjectionType::Perspective),
    viewMatrix(1.0f),
    projectionMatrix(1.0f)
{
    computeVectors();
    computeView();
    computeProjection();
    notify();
}

void Camera::computeVectors() {
    target.x = std::sin(alpha) * std::cos(fi);
    target.z = std::sin(alpha) * std::sin(fi);
    target.y = std::cos(alpha);

    target = glm::normalize(target);
}

void Camera::computeView() {
    viewMatrix = glm::lookAt(eye, eye + target, up);
}

void Camera::computeProjection() {
    if (projType == ProjectionType::Perspective) {
        projectionMatrix = glm::perspective(fov, aspect, zNear, zFar);
    }
    else {
        projectionMatrix = glm::ortho(
            -orthoHalfHeight * aspect,
            orthoHalfHeight * aspect,
            -orthoHalfHeight,
            orthoHalfHeight,
            zNear,
            zFar
        );
    }
}

void Camera::updateVectors() {
    computeVectors();
    updateView();
}

void Camera::updateView() {
    computeView();
    notify();
}

void Camera::updateProjection() {
    computeProjection();
    notify();
}

void Camera::attach(Observer* observer) {
    if (observer && std::find(observers.begin(), observers.end(), observer) == observers.end()) {
        observers.push_back(observer);
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
    if (projType != type) {
        projType = type;
        updateProjection();
    }
}

void Camera::setPose(const glm::vec3& newEye, float newAlpha, float newFi) {
    eye = newEye;
    alpha = newAlpha;
    fi = newFi;

    const float eps = 0.01f;
    if (alpha < eps) alpha = eps;
    if (alpha > glm::pi<float>() - eps) alpha = glm::pi<float>() - eps;

    updateVectors();
}

glm::mat4 Camera::getViewMatrix() const { return viewMatrix; }
glm::mat4 Camera::getProjectionMatrix() const { return projectionMatrix; }
glm::vec3 Camera::getPosition() const { return eye; }