/**
 * @file Transformation.cpp
 * @author Student (login: ZHA0067)
 * @brief Implementation of 4x4 matrix transformation methods.
 */

#include "Transformation.h"

Translation::Translation(const glm::vec3& vec) : translationVector(vec) {}
Translation::Translation(float x, float y, float z) : translationVector(x, y, z) {}

void Translation::setPosition(const glm::vec3& vec) { translationVector = vec; }
void Translation::setPosition(float x, float y, float z) { translationVector = glm::vec3(x, y, z); }
glm::vec3 Translation::getPosition() const { return translationVector; }

glm::mat4 Translation::getMatrix() const {
    return glm::translate(glm::mat4(1.0f), translationVector);
}

Rotation::Rotation(float angleInRadians, const glm::vec3& rotationAxis)
    : angle(angleInRadians), axis(rotationAxis) {}

void Rotation::setAngle(float angleInRadians) { angle = angleInRadians; }
void Rotation::addAngle(float deltaRadians) { angle += deltaRadians; }
float Rotation::getAngle() const { return angle; }
glm::vec3 Rotation::getAxis() const { return axis; }

glm::mat4 Rotation::getMatrix() const {
    return glm::rotate(glm::mat4(1.0f), angle, axis);
}

Scale::Scale(const glm::vec3& scale) : scaleVector(scale) {}
Scale::Scale(float uniformScale) : scaleVector(uniformScale) {}
Scale::Scale(float sx, float sy, float sz) : scaleVector(sx, sy, sz) {}

void Scale::setScale(const glm::vec3& scale) { scaleVector = scale; }
void Scale::setScale(float uniformScale) { scaleVector = glm::vec3(uniformScale); }
glm::vec3 Scale::getScale() const { return scaleVector; }

glm::mat4 Scale::getMatrix() const {
    return glm::scale(glm::mat4(1.0f), scaleVector);
}

void CompositeTransformation::add(std::shared_ptr<Transformation> t) {
    if (t) {
        transformations.push_back(t);
    }
}

void CompositeTransformation::clear() {
    transformations.clear();
}

const std::vector<std::shared_ptr<Transformation>>& CompositeTransformation::getChildren() const {
    return transformations;
}

glm::mat4 CompositeTransformation::getMatrix() const {
    glm::mat4 result = glm::mat4(1.0f);
    for (const auto& t : transformations) {
        if (t) {
            result = result * t->getMatrix();
        }
    }
    return result;
}