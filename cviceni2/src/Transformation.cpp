/**
 * @file Transformation.cpp
 * @author Student (login: ZHA0067)
 * @brief Implementation of 4x4 matrix transformation methods.
 */

#include "Transformation.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

LeafTransform::LeafTransform(const glm::mat4& m) : matrix(m) {}

glm::mat4 LeafTransform::getMatrix() const {
    return matrix;
}

void LeafTransform::setMatrix(const glm::mat4& m) {
    matrix = m;
}


void CompositeTransform::add(const std::shared_ptr<TransformComponent>& component) {
    if (component) {
        components.push_back(component);
    }
}

void CompositeTransform::clear() {
    components.clear();
}

glm::mat4 CompositeTransform::getMatrix() const {
    glm::mat4 result = glm::mat4(1.0f);
    for (const auto& comp : components) {
        result = result * comp->getMatrix();
    }
    return result;
}


Transformation::Transformation() : modelMatrix(glm::mat4(1.0f)) {}

Transformation::Transformation(const glm::mat4& m) : modelMatrix(m) {}

void Transformation::reset() {
    modelMatrix = glm::mat4(1.0f);
}

void Transformation::translate(const glm::vec3& translation) {
    modelMatrix = glm::translate(modelMatrix, translation);
}

void Transformation::rotate(float angleRadians, const glm::vec3& axis) {
    modelMatrix = glm::rotate(modelMatrix, angleRadians, axis);
}

void Transformation::scale(const glm::vec3& scaling) {
    modelMatrix = glm::scale(modelMatrix, scaling);
}

void Transformation::setMatrix(const glm::mat4& m) {
    modelMatrix = m;
}

const glm::mat4& Transformation::getModelMatrix() const {
    return modelMatrix;
}