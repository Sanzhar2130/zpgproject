/**
 * @file DrawableObject.h
 * @author Student (login: ZHA0067)
 * @brief Declaration of visual entity associating Model, ShaderProgram, and Transformation.
 */
#pragma once

#include <glm/glm.hpp>
#include "Model.h"
#include "ShaderProgram.h"
#include "Transformation.h"

class DrawableObject {
private:
    Model* model;
    ShaderProgram* shaderProgram;
    Transformation transformation;
    glm::vec3 color;

public:
    DrawableObject(Model* m, ShaderProgram* sp,
        const Transformation& t = Transformation(),
        const glm::vec3& c = glm::vec3(1.0f));

    void draw(const glm::mat4& viewMatrix, const glm::mat4& projectMatrix);

    Transformation& getTransformation();
    const Transformation& getTransformation() const;
    void setTransformation(const Transformation& t);

    void setColor(const glm::vec3& c);
    glm::vec3 getColor() const;
};