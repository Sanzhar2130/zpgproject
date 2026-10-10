/**
 * @file Scene.h
 * @author Student (login: ZHA0067)
 * @brief Container for static scene objects and their rendering.
 */
#pragma once

#include <vector>
#include <string>
#include "DrawableObject.h"

class Scene {
private:
    std::string name;
    std::vector<DrawableObject*> objects;

public:
    Scene(const std::string& sceneName);
    ~Scene();

    void addObject(DrawableObject* object);
    void render() const;

    const std::string& getName() const;
    const std::vector<DrawableObject*>& getObjects() const;
};