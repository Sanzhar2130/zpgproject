/**
 * @file Rotation.h
 * @author Student (login: ZHA0067)
 * @brief Leaf transformation representing rotation around an arbitrary axis.
 */
#pragma once

#include <glm/glm.hpp>
#include "Transformation.h"

class Rotation : public Transformation {
private:
    float angle;
    glm::vec3 axis;

public:
    Rotation(float angleInRadians, const glm::vec3& rotationAxis);

    void setAngle(float angleInRadians);
    void addAngle(float deltaRadians);
    float getAngle() const;
    glm::vec3 getAxis() const;

    glm::mat4 getMatrix() const override;
};