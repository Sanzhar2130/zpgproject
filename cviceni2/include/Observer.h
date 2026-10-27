/**
 * @file Observer.h
 * @author Student (login: ZHA0067)
 * @brief Base class for observer pattern implementation.
 */
#pragma once
#include <glm/glm.hpp>

class Camera;

class Observer {
public:
    virtual ~Observer() = default;
    virtual void update(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) = 0;
};