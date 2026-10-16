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
    // Вычисление вектора направления взгляда по формулам из лекции ZPG
    target.x = sin(alpha) * cos(fi);
    target.z = sin(alpha) * sin(fi);
    target.y = cos(alpha);

    target = glm::normalize(target);
    updateView();
}

void Camera::updateView() {
    // Формирование матрицы вида: LookAt(глаз, куда смотрим, где верх)
    viewMatrix = glm::lookAt(eye, eye + target, up);
    notify();
}

void Camera::updateProjection() {
    if (projType == ProjectionType::Perspective) {
        // zNear строго 0.1f (не 0.0f, чтобы исключить деление на ноль в Z-буфере)
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
        // Немедленно передаем текущее состояние новому наблюдателю
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

    // Защита от Gimbal Lock: ограничиваем зенитный угол alpha
    // Угол не должен достигать 0 или 180 градусов (в радианах ~0.01f и ~3.13f)
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