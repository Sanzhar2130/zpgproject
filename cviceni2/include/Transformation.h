/**
 * @file Transformation.h
 * @author Student (login: ZHA0067)
 * @brief Composite Pattern implementation for 4x4 matrix transformations using GLM.
 */
#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <memory>


class Transformation {
public:
    virtual ~Transformation() = default;
    virtual glm::mat4 getMatrix() const = 0;
};


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

class Rotation : public Transformation {
private:
    float angle;
    glm::vec3 axis;

public:
    Rotation(float angleInRadians, const glm::vec3& rotationAxis);

    void setAngle(float angleInRadians);
    void addAngle(float deltaRadians);
    float getAngle() const;
    glm::vec3 getAxis() const;

    glm::mat4 getMatrix() const override;
};

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