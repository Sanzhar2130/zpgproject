/**
 * @file CompositeTransformation.h
 * @author Student (login: ZHA0067)
 * @brief Composite node container combining multiple Transformation instances.
 */
#pragma once

#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include "Transformation.h"

class CompositeTransformation : public Transformation {
private:
    std::vector<std::shared_ptr<Transformation>> transformations;

public:
    CompositeTransformation() = default;

    void add(std::shared_ptr<Transformation> t);
    void clear();
    const std::vector<std::shared_ptr<Transformation>>& getChildren() const;

    glm::mat4 getMatrix() const override;
};