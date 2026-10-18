/**
 * @file Translation.cpp
 * @brief Implementation of Translation transformation.
 */
#include "Translation.h"
#include <glm/gtc/matrix_transform.hpp>

Translation::Translation(const glm::vec3& vec) : translationVector(vec) {}
Translation::Translation(float x, float y, float z) : translationVector(x, y, z) {}

void Translation::setPosition(const glm::vec3& vec) { translationVector = vec; }
void Translation::setPosition(float x, float y, float z) { translationVector = glm::vec3(x, y, z); }
glm::vec3 Translation::getPosition() const { return translationVector; }

glm::mat4 Translation::getMatrix() const {
    return glm::translate(glm::mat4(1.0f), translationVector);
}