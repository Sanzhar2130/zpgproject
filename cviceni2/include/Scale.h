/**
 * @file Scale.h
 * @author Student (login: ZHA0067)
 * @brief Leaf transformation representing scaling along axes.
 */
#pragma once

#include <glm/glm.hpp>
#include "Transformation.h"

class Scale : public Transformation {
private:
    glm::vec3 scaleVector;

public:
    Scale(const glm::vec3& scale);
    Scale(float uniformScale);
    Scale(float sx, float sy, float sz);

    void setScale(const glm::vec3& scale);
    void setScale(float uniformScale);
    glm::vec3 getScale() const;

    glm::mat4 getMatrix() const override;
};