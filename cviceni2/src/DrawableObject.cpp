/**
 * @file DrawableObject.cpp
 * @author Student (login: ZHA0067)
 * @brief Implementation of DrawableObject rendering using Transformation getters.
 */
#include "DrawableObject.h"

DrawableObject::DrawableObject(Model* m, ShaderProgram* sp,
    std::shared_ptr<Transformation> t,
    const glm::vec3& c,
    GLuint texID)
    : model(m), shaderProgram(sp), transformation(t), color(c), textureID(texID) {}

void DrawableObject::draw() const {
    if (!model || !shaderProgram) return;

    shaderProgram->use();

    if (textureID != 0) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID);
        shaderProgram->setUniform("textureUnitID", 0);
    }

    glm::mat4 modelMat = transformation ? transformation->getMatrix() : glm::mat4(1.0f);
    shaderProgram->setUniform("modelMatrix", modelMat);
    shaderProgram->setUniform("uColor", color);

    model->draw();

    glUseProgram(0);
}