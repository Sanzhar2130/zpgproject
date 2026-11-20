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
#include "MovingObjectController.h"
#include "ScenePicker.h"
#include "InputController.h"
#include "ModelFactory.h"
#include "ShaderFactory.h"
#include "SceneFactory.h"
#include "ScenePicker.h"
#include "AnimationController.h"
#include "LightController.h"
#include "SkyboxRenderer.h"

class Application {
private:
    GLFWwindow* window = nullptr;
    int windowWidth = 1024;
    int windowHeight = 768;

    std::vector<Scene*> scenes;
    size_t activeSceneIndex = 0;

    std::vector<Model*> models;
    std::vector<ShaderProgram*> shaders;

    ShaderProgram* clockShader;
    ShaderProgram* treeShader;
    ShaderProgram* phongShader;
    ShaderProgram* blinnShader;
    ShaderProgram* ambientShader;
    ShaderProgram* blinnColorShader;
    ShaderProgram* nightShader;
    ShaderProgram* textureShader;
    ShaderProgram* skyboxShader = nullptr;

    GLuint skyboxTex = 0;
    GLuint shrekTex = 0;
    GLuint tableTex = 0;
    GLuint ocTex = 0;
    GLuint shrekebTex = 0;

    Camera* camera = nullptr;
    MovingObjectController movingController;
    ScenePicker scenePicker;
    AnimationController animationController;
    LightController lightController;
    SkyboxRenderer skyboxRenderer;
    SceneAnimRefs animRefs;
    InputController inputController;

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    static void error_callback(int error, const char* description);
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void window_size_callback(GLFWwindow* window, int width, int height);
    static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos);
    static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

public:
    Application() = default;
    ~Application();

    void initialization();
    void createShaders();
    void createModels();
    void createScenes();
    void run();
};