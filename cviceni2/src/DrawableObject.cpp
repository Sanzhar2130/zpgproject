/**
 * @file DrawableObject.cpp
 * @author Student (login: ZHA0067)
 * @brief Implementation of DrawableObject rendering using Transformation getters.
 */
#include "DrawableObject.h"

DrawableObject::DrawableObject(Model* m, ShaderProgram* sp,
    std::shared_ptr<Transformation> t,
    const glm::vec3& c)
    : model(m), shaderProgram(sp), transformation(t), color(c) {}

void DrawableObject::draw() const {
    if (!model || !shaderProgram) return;

    shaderProgram->use();

    glm::mat4 modelMat = transformation ? transformation->getMatrix() : glm::mat4(1.0f);
    shaderProgram->setUniform("modelMatrix", modelMat);
    shaderProgram->setUniform("uColor", color);

    model->draw();

    glUseProgram(0);
}