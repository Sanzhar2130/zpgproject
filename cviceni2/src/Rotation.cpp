/**
 * @file Rotation.cpp
 * @brief Implementation of Rotation transformation.
 */
#include "Rotation.h"
#include <glm/gtc/matrix_transform.hpp>

Rotation::Rotation(float angleInRadians, const glm::vec3& rotationAxis)
    : angle(angleInRadians), axis(rotationAxis) {}

void Rotation::setAngle(float angleInRadians) { angle = angleInRadians; }
void Rotation::addAngle(float deltaRadians) { angle += deltaRadians; }
float Rotation::getAngle() const { return angle; }
glm::vec3 Rotation::getAxis() const { return axis; }

glm::mat4 Rotation::getMatrix() const {
    return glm::rotate(glm::mat4(1.0f), angle, axis);
}