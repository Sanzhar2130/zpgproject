/**
 * @file Scale.cpp
 * @brief Implementation of Scale transformation.
 */
#include "Scale.h"
#include <glm/gtc/matrix_transform.hpp>

Scale::Scale(const glm::vec3& scale) : scaleVector(scale) {}
Scale::Scale(float uniformScale) : scaleVector(uniformScale) {}
Scale::Scale(float sx, float sy, float sz) : scaleVector(sx, sy, sz) {}

void Scale::setScale(const glm::vec3& scale) { scaleVector = scale; }
void Scale::setScale(float uniformScale) { scaleVector = glm::vec3(uniformScale); }
glm::vec3 Scale::getScale() const { return scaleVector; }

glm::mat4 Scale::getMatrix() const {
    return glm::scale(glm::mat4(1.0f), scaleVector);
}