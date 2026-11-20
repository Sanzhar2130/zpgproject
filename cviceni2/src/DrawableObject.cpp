/**
 * @file DrawableObject.cpp
 * @author Student (login: ZHA0067)
 * @brief Implementation of DrawableObject rendering using Transformation getters.
 */
#include "DrawableObject.h"
#include <glm/gtc/matrix_inverse.hpp>

DrawableObject::DrawableObject(Model* m, ShaderProgram* sp,
    std::shared_ptr<Transformation> t,
    const glm::vec3& c,
    GLuint texID,
    GLuint objectID)
    : model(m), shaderProgram(sp), transformation(t), color(c), textureID(texID), id(objectID), selected(false) {}

void DrawableObject::draw() const {
    if (!model || !shaderProgram) return;

    shaderProgram->use();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);
    if (textureID != 0) {
        shaderProgram->setUniform("textureUnitID", 0);
    }

    glm::mat4 modelMat = transformation ? transformation->getMatrix() : glm::mat4(1.0f);
    shaderProgram->setUniform("modelMatrix", modelMat);

    glm::mat3 normalMat = glm::transpose(glm::inverse(glm::mat3(modelMat)));
    shaderProgram->setUniform("normalMatrix", normalMat);

    glm::vec3 renderColor = selected ? glm::vec3(1.0f, 0.85f, 0.2f) : color;
    shaderProgram->setUniform("uColor", renderColor);

    glStencilFunc(GL_ALWAYS, id, 0xFF);

    model->draw();

    glUseProgram(0);
}