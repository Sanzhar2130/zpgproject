/**
 * @file DrawableObject.h
 * @author Student (login: ZHA0067)
 * @brief Declaration of visual entity associating Model, ShaderProgram, and Transformation.
 */
#pragma once

#include <glad/gl.h>
#include <glm/glm.hpp>
#include <memory>
#include "Model.h"
#include "ShaderProgram.h"
#include "Transformation.h"

class DrawableObject {
private:
    Model* model;
    ShaderProgram* shaderProgram;
    std::shared_ptr<Transformation> transformation;
    glm::vec3 color;
    bool isWatermark;

public:
    DrawableObject(Model* m, ShaderProgram* sp,
        std::shared_ptr<Transformation> t = nullptr,
        const glm::vec3& c = glm::vec3(1.0f));
    ~DrawableObject();

    void draw() const;

    void setTransformation(std::shared_ptr<Transformation> t);
    std::shared_ptr<Transformation> getTransformation() const;

    void setColor(const glm::vec3& c);
    glm::vec3 getColor() const;

    void setWatermark(bool watermark);
    bool getWatermark() const;
};