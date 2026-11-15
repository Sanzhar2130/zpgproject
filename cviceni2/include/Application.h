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

enum class InteractionMode {
    Select,
    PlaceStatic,
    PlaceMoving
};

struct MovingObject {
    size_t sceneIndex;
    GLuint objectID;
    std::shared_ptr<Translation> translation;
    glm::vec3 pointA;
    glm::vec3 pointB;
    float t;
    float speed;
    int direction;
};

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
    ShaderProgram* phongShader;
    ShaderProgram* blinnShader;
    ShaderProgram* ambientShader;
    ShaderProgram* blinnColorShader;
    ShaderProgram* vertPhongBad;
    ShaderProgram* fragPhongBad;
    ShaderProgram* nightShader;
    ShaderProgram* textureShader;
    ShaderProgram* skyboxShader = nullptr;
    GLuint skyboxTex = 0;

    GLuint shrekTex = 0;
    GLuint tableTex = 0;
    GLuint ocTex = 0;
	GLuint shrekebTex = 0;

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

    std::shared_ptr<Rotation> rocketOrbitRotation;
    std::shared_ptr<Rotation> rocketSelfRotation;

    InteractionMode interactionMode = InteractionMode::Select;
    bool pendingClickAction = false;
    bool forceStaticPlaceOnce = false;
    bool placeAlternateToggle = true;
    double pickCursorX = 0.0;
    double pickCursorY = 0.0;

    bool hasFirstLinePoint = false;
    glm::vec3 linePointA = glm::vec3(0.0f);
    std::vector<MovingObject> movingObjects;

    void performPicking();

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