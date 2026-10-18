/**
 * @file Transformation.h
 * @author Student (login: ZHA0067)
 * @brief Composite Pattern implementation for 4x4 matrix transformations using GLM.
 */
#pragma once

#include <glm/glm.hpp>

class Transformation {
public:
    virtual ~Transformation() = default;
    virtual glm::mat4 getMatrix() const = 0;
};