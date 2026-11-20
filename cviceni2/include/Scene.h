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
    GLuint nextID;
    DrawableObject* selectedObject;

public:
    Scene(const std::string& sceneName);
    ~Scene();

    Scene(const Scene&) = delete;
    Scene& operator=(const Scene&) = delete;

    void addObject(DrawableObject* object);
    bool removeObjectByID(GLuint id);
    void render() const;

    void selectByID(GLuint id);
    void setSelect(int index);
    DrawableObject* getObjectByID(GLuint id) const;
    DrawableObject* getSelectedObject() const;
    GLuint getSelectedID() const;

    const std::string& getName() const;
    const std::vector<DrawableObject*>& getObjects() const;
};