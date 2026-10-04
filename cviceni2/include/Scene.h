/**
 * @file Scene.h
 * @author Student (login: ZHA0067)
 * @brief Container for static scene objects and their rendering.
 */
#pragma once

#include <vector>
#include <string>
#include <glm/glm.hpp>
#include "DrawableObject.h"

class Scene {
private:
    std::string name;
    std::vector<DrawableObject*> objects;
    glm::mat4 viewMatrix;
    glm::mat4 projectMatrix;

public:
    Scene(const std::string& sceneName);
    ~Scene();

    void addObject(DrawableObject* obj);
    void setMatrices(const glm::mat4& view, const glm::mat4& projection);
    void render();

    const std::vector<DrawableObject*>& getObjects() const;
    const std::string& getName() const;
};