/**
 * @file Application.h
 * @author Student (login: ZHA0067)
 * @brief Application lifecycle coordinator, GLFW manager, and scene switcher.
 */
#pragma once

#include <vector>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Scene.h"
#include "Model.h"
#include "ShaderProgram.h"
#include "DrawableObject.h"

class Application {
private:
    GLFWwindow* window;
    int windowWidth;
    int windowHeight;

    std::vector<Scene*> scenes;
    size_t activeSceneIndex;

    std::vector<Model*> models;
    std::vector<ShaderProgram*> shaders;

    DrawableObject* sunObject;
    DrawableObject* earthObject;
    DrawableObject* moonObject;
    DrawableObject* marsObject;

    static void error_callback(int error, const char* description);
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void window_size_callback(GLFWwindow* window, int width, int height);
	static void cursor_callback(GLFWwindow* window, double x, double y);
	static void button_callback(GLFWwindow* window, int button, int action, int mode);

    void updateSolarSystem(float currentTime);
public:
    Application();
    ~Application();

    void initialization();
    void createShaders();
    void createModels();
    void createScenes();
    void run();

    void handleKeyboard(int key, int action);
};