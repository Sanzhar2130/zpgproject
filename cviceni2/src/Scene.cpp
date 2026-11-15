/**
 * @file Scene.cpp
 * @author Student (login: ZHA0067)
 * @brief Implementation of Scene container.
 */
#include "Scene.h"
#include <iostream>
#include <algorithm>

Scene::Scene(const std::string& sceneName) : name(sceneName), nextID(1), selectedObject(nullptr) {}

Scene::~Scene() {
    for (DrawableObject* obj : objects) {
        delete obj;
    }
    objects.clear();
    selectedObject = nullptr;
}

void Scene::addObject(DrawableObject* object) {
    if (object == nullptr) return;

    if (object->getID() == 0) {
        if (nextID <= 255) {
            object->setID(nextID++);
        }
        else {
            std::cerr << "Warning: Stencil buffer ID limit (255) reached in scene: " << name << std::endl;
            object->setID(255);
        }
    }
    else {
        if (object->getID() >= nextID && object->getID() < 255) {
            nextID = object->getID() + 1;
        }
    }

    objects.push_back(object);
}

bool Scene::removeObjectByID(GLuint id) {
    if (id == 0) return false;

    for (auto it = objects.begin(); it != objects.end(); ++it) {
        DrawableObject* obj = *it;
        if (obj != nullptr && obj->getID() == id) {
            if (selectedObject == obj) {
                selectedObject = nullptr;
            }
            delete obj;
            objects.erase(it);
            return true;
        }
    }
    return false;
}

void Scene::render() const {
    for (const DrawableObject* obj : objects) {
        if (obj != nullptr) {
            obj->draw();
        }
    }
}

void Scene::selectByID(GLuint id) {
    if (selectedObject != nullptr) {
        selectedObject->setSelected(false);
        selectedObject = nullptr;
    }

    if (id == 0) return;

    DrawableObject* found = getObjectByID(id);
    if (found != nullptr) {
        found->setSelected(true);
        selectedObject = found;
    }
}

void Scene::setSelect(int index) {
    if (selectedObject != nullptr) {
        selectedObject->setSelected(false);
        selectedObject = nullptr;
    }

    if (index >= 0 && static_cast<size_t>(index) < objects.size()) {
        objects[index]->setSelected(true);
        selectedObject = objects[index];
    }
}

DrawableObject* Scene::getObjectByID(GLuint id) const {
    if (id == 0) return nullptr;

    for (DrawableObject* obj : objects) {
        if (obj != nullptr && obj->getID() == id) {
            return obj;
        }
    }
    return nullptr;
}

DrawableObject* Scene::getSelectedObject() const {
    return selectedObject;
}

GLuint Scene::getSelectedID() const {
    return selectedObject ? selectedObject->getID() : 0;
}

const std::string& Scene::getName() const {
    return name;
}

const std::vector<DrawableObject*>& Scene::getObjects() const {
    return objects;
}