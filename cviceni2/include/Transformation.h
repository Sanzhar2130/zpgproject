/**
 * @file Transformation.h
 * @author Student (login: ZHA0067)
 * @brief Composite Pattern implementation for 4x4 matrix transformations using GLM.
 */
#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <memory>

class TransformComponent {
public:
    virtual ~TransformComponent() = default;
    virtual glm::mat4 getMatrix() const = 0;
};

class LeafTransform : public TransformComponent {
private:
    glm::mat4 matrix;

public:
    explicit LeafTransform(const glm::mat4& m = glm::mat4(1.0f));
    glm::mat4 getMatrix() const override;
    void setMatrix(const glm::mat4& m);
};


class CompositeTransform : public TransformComponent {
private:
    std::vector<std::shared_ptr<TransformComponent>> components;

public:
    CompositeTransform() = default;
    void add(const std::shared_ptr<TransformComponent>& component);
    void clear();
    glm::mat4 getMatrix() const override;
};


class Transformation {
private:
    glm::mat4 modelMatrix;

public:
    Transformation();
    explicit Transformation(const glm::mat4& m);

    void reset();
    void translate(const glm::vec3& translation);
    void rotate(float angleRadians, const glm::vec3& axis);
    void scale(const glm::vec3& scaling);

    void setMatrix(const glm::mat4& m);
    const glm::mat4& getModelMatrix() const;
};