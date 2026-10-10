/**
 * @file DrawableObject.cpp
 * @author Student (login: ZHA0067)
 * @brief Implementation of DrawableObject rendering using Transformation getters.
 */
#include "DrawableObject.h"
#include "Transformation.h" 
#include <glm/glm.hpp>

DrawableObject::DrawableObject(Model* m, ShaderProgram* sp,
    std::shared_ptr<Transformation> t,
    const glm::vec3& c)
    : model(m), shaderProgram(sp), transformation(t),
      color(c), isWatermark(false)
{
}

DrawableObject::~DrawableObject() = default;

void DrawableObject::draw() const {
    if (shaderProgram) {
        shaderProgram->use();

        if (transformation) {
            shaderProgram->setUniform("modelMatrix", transformation->getMatrix());
        } else {
            shaderProgram->setUniform("modelMatrix", glm::mat4(1.0f));
        }

        shaderProgram->setUniform("uColor", color);
    }

    if (model) {
        model->draw();
    }
}

void DrawableObject::setTransformation(std::shared_ptr<Transformation> t) {
    transformation = t;
}

std::shared_ptr<Transformation> DrawableObject::getTransformation() const {
    return transformation;
}
        
void DrawableObject::setColor(const glm::vec3& c) { color = c; }
glm::vec3 DrawableObject::getColor() const { return color; }

void DrawableObject::setWatermark(bool watermark) { isWatermark = watermark; }
bool DrawableObject::getWatermark() const { return isWatermark; }