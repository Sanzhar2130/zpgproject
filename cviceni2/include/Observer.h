#pragma once
#include <glm/glm.hpp>

class Camera;

class Observer {
public:
    virtual ~Observer() = default;
    virtual void update(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) = 0;
};