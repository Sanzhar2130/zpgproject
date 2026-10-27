/**
 * @file Translation.h
 * @author Student (login: ZHA0067)
 * @brief Leaf transformation representing position translation.
 */
#pragma once

#include <glm/glm.hpp>
#include "Transformation.h"

class Translation : public Transformation {
private:
    glm::vec3 translationVector;

public:
    Translation(const glm::vec3& vec);
    Translation(float x, float y, float z);

    void setPosition(const glm::vec3& vec);
    void setPosition(float x, float y, float z);
    glm::vec3 getPosition() const;

    glm::mat4 getMatrix() const override;
};