/**
 * @file CompositeTransformation.cpp
 * @brief Implementation of CompositeTransformation accumulating matrix multiplication.
 */
#include "CompositeTransformation.h"

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