/**
 * @file Application.h
 * @author Student (login: ZHA0067)
 * @brief Application lifecycle coordinator, GLFW manager, and scene switcher.
 */
#pragma once

#include <vector>
#include <memory>
#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "Scene.h"
#include "Model.h"
#include "ShaderProgram.h"
#include "Transformation.h"
#include "Camera.h"
#include "Translation.h"
#include "Rotation.h"
#include "Scale.h"
#include "CompositeTransformation.h"

class Application {
private:
    GLFWwindow* window;
    int windowWidth;
    int windowHeight;

    std::vector<Scene*> scenes;
    size_t activeSceneIndex;

    std::vector<Model*> models;
    std::vector<ShaderProgram*> shaders;

    ShaderProgram* clockShader;
    ShaderProgram* treeShader;

    Camera* camera;

    double lastX;
    double lastY;
	bool firstMouse;
    bool isMousePressed;

    float deltaTime;
    float lastFrame;

    std::shared_ptr<Rotation> triRotLocal;
    std::shared_ptr<Rotation> triRotOrbit;

    std::shared_ptr<Rotation> sunSelfRotation;
    std::shared_ptr<Rotation> earthOrbitRotation;
    std::shared_ptr<Rotation> earthSelfRotation;
    std::shared_ptr<Rotation> moonOrbitRotation;
    std::shared_ptr<Rotation> moonSelfRotation;

    static void error_callback(int error, const char* description);
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void window_size_callback(GLFWwindow* window, int width, int height);
    static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos);
    static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods); 

    void processInput();
    void addWatermark(Scene* scene);

public:
    Application();
    ~Application();

    void initialization();
    void createShaders();
    void createModels();
    void createScenes();
    void run();

    void handleKeyboard(int key, int action);
    void handleMouse(double xpos, double ypos);
    void handleMouseButton(int button, int action, int mods);
};