/**
 * @file DrawableObject.cpp
 * @author Student (login: ZHA0067)
 * @brief Implementation of drawing routines and parameter dispatching to GPU.
 */
#include "DrawableObject.h"

DrawableObject::DrawableObject(Model* m, ShaderProgram* sp,
    const Transformation& t,
    const glm::vec3& c)
    : model(m), shaderProgram(sp), transformation(t), color(c) {}

void DrawableObject::draw(const glm::mat4& viewMatrix, const glm::mat4& projectMatrix) {
    if (!model || !shaderProgram) return;

    shaderProgram->use();

    shaderProgram->setUniformMatrix4fv("modelMatrix", transformation.getModelMatrix());
    shaderProgram->setUniformMatrix4fv("viewMatrix", viewMatrix);
    shaderProgram->setUniformMatrix4fv("projectMatrix", projectMatrix);

    shaderProgram->setUniform3f("uColor", color);

    model->draw();

    glUseProgram(0);
}

Transformation& DrawableObject::getTransformation() {
    return transformation;
}

const Transformation& DrawableObject::getTransformation() const {
    return transformation;
}

void DrawableObject::setTransformation(const Transformation& t) {
    transformation = t;
}

void DrawableObject::setColor(const glm::vec3& c) {
    color = c;
}

glm::vec3 DrawableObject::getColor() const {
    return color;
}