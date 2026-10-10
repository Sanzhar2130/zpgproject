/**
 * @file Scene.cpp
 * @author Student (login: ZHA0067)
 * @brief Implementation of Scene container.
 */
#include "Scene.h"

Scene::Scene(const std::string& sceneName) : name(sceneName) {}

Scene::~Scene() {
    for (DrawableObject* obj : objects) {
        delete obj;
    }
    objects.clear();
}

void Scene::addObject(DrawableObject* object) {
    if (object != nullptr) {
        objects.push_back(object);
    }
}

void Scene::render() const {
    for (const DrawableObject* obj : objects) {
        if (obj != nullptr) {
            obj->draw();
        }
    }
}

const std::string& Scene::getName() const {
    return name;
}

const std::vector<DrawableObject*>& Scene::getObjects() const {
    return objects;
}