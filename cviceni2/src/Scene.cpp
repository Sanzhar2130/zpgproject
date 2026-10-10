/**
 * @file Scene.cpp
 * @author Student (login: ZHA0067)
 * @brief Implementation of Scene container.
 */
#include "Scene.h"

Scene::Scene(const std::string& sceneName) 
    : name(sceneName), viewMatrix(glm::mat4(1.0f)), projectMatrix(glm::mat4(1.0f)) {}

Scene::~Scene() {
    for (DrawableObject* obj : objects) {
        delete obj;
    }
    objects.clear();
}

void Scene::addObject(DrawableObject* obj) {
    if (obj) {
        objects.push_back(obj);
    }
}

void Scene::setMatrices(const glm::mat4& view, const glm::mat4& projection) {
    viewMatrix = view;
    projectMatrix = projection;
}

void Scene::render() {
    for (DrawableObject* obj : objects) {
        if (obj) {
            obj->draw(viewMatrix, projectMatrix);
        }
    }
}

const std::vector<DrawableObject*>& Scene::getObjects() const {
    return objects;
}

const std::string& Scene::getName() const {
    return name;
}