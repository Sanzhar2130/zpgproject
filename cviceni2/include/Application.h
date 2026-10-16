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

    // Интерактивная камера сцены
    Camera* camera;

    // Переменные для обработки мыши
    double lastX;
    double lastY;
    bool firstMouse;

    // Дельта времени для плавной ходьбы независимо от FPS
    float deltaTime;
    float lastFrame;

    // Динамические трансформации сцены 1 (T*R против R*T)
    std::shared_ptr<Rotation> triRotLocal;
    std::shared_ptr<Rotation> triRotOrbit;

    // Динамические трансформации сцены 5 (Солнечная система)
    std::shared_ptr<Rotation> sunSelfRotation;
    std::shared_ptr<Rotation> earthOrbitRotation;
    std::shared_ptr<Rotation> earthSelfRotation;
    std::shared_ptr<Rotation> moonOrbitRotation;
    std::shared_ptr<Rotation> moonSelfRotation;

    // Callback-функции библиотеки GLFW
    static void error_callback(int error, const char* description);
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void window_size_callback(GLFWwindow* window, int width, int height);
    static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos);

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
};