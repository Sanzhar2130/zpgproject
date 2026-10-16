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
    // Позиция и ориентация в пространстве
    glm::vec3 eye;
    glm::vec3 target;
    glm::vec3 up;

    // Сферические углы (из лекции ZPG)
    // alpha - зенитный угол (от оси Y), fi - азимутальный угол (в плоскости XZ)
    float alpha;
    float fi;

    // Параметры проекции
    float fov;
    float aspect;
    float zNear;
    float zFar;
    ProjectionType projType;

    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;

    // Список наблюдателей (Shaders)
    std::vector<Observer*> observers;

    void updateVectors();
    void updateProjection();
    void updateView();

public:
    Camera(glm::vec3 startEye = glm::vec3(0.0f, 2.0f, 5.0f),
        float startAlpha = glm::radians(90.0f),
        float startFi = glm::radians(-90.0f),
        float screenAspect = 4.0f / 3.0f);

    // Паттерн Observer
    void attach(Observer* observer);
    void detach(Observer* observer);
    void notify();

    // Управление ориентацией (мышь)
    void handleMouseMovement(float xOffset, float yOffset, float sensitivity = 0.005f);

    // Перемещение (клавиатура WASD)
    void moveForward(float delta);
    void moveBackward(float delta);
    void moveLeft(float delta);
    void moveRight(float delta);

    // Настройка проекции
    void setAspect(float newAspect);
    void setProjectionType(ProjectionType type);

    // Геттеры матриц
    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix() const;
    glm::vec3 getPosition() const;
};