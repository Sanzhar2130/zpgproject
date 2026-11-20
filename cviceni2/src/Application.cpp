/**
 * @file Application.cpp
 * @author Student (login: ZHA0067)
 * @brief Application lifecycle coordinator with static scenes and scene switcher.
 */

#include "Application.h"
#include <iostream>
#include <glm/gtc/constants.hpp>
#include <ctime>


static Application* getApp(GLFWwindow* window) {
    return static_cast<Application*>(glfwGetWindowUserPointer(window));
}

void Application::error_callback(int error, const char* description) {
    std::cerr << "GLFW Error [" << error << "]: " << description << std::endl;
}

void Application::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (auto* app = getApp(window)) {
        app->inputController.onKeyboard(
            window, key, action, app->camera,
            app->scenes, app->activeSceneIndex,
            app->scenePicker, app->movingController
        );
    }
}

void Application::window_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    if (auto* app = getApp(window)) {
        app->windowWidth = width;
        app->windowHeight = height;
        if (app->camera && height > 0) {
            app->camera->setAspect(static_cast<float>(width) / static_cast<float>(height));
        }
    }
}

void Application::cursor_pos_callback(GLFWwindow* window, double xpos, double ypos) {
    if (auto* app = getApp(window)) {
        app->inputController.onMouseMove(xpos, ypos, app->camera);
    }
}
void Application::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (auto* app = getApp(window)) {
        app->inputController.onMouseButton(window, button, action, mods, app->scenePicker);
    }
}

Application::~Application() {
    for (Scene* s : scenes) delete s;
    scenes.clear();

    for (Model* m : models) delete m;
    models.clear();

    for (ShaderProgram* sp : shaders) delete sp;
    shaders.clear();

	delete camera;
	camera = nullptr;

    if (window) glfwDestroyWindow(window);
    glfwTerminate();
}

void Application::initialization() {
    glfwSetErrorCallback(error_callback);

    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_STENCIL_BITS, 8);

    window = glfwCreateWindow(windowWidth, windowHeight, "ZPG (ZHA0067)", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetWindowUserPointer(window, this);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    glfwSetKeyCallback(window, key_callback);
    glfwSetWindowSizeCallback(window, window_size_callback);
    glfwSetCursorPosCallback(window, cursor_pos_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL Version:   " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

    float aspect = static_cast<float>(windowWidth) / static_cast<float>(windowHeight);
    camera = new Camera(glm::vec3(0.0f, 2.0f, 6.0f), glm::radians(90.0f), glm::radians(-90.0f), aspect);

    int maxUnits = 0;
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxUnits);
    std::cout << "Texture units available in Fragment Shader: " << maxUnits << std::endl;

    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
}

void Application::createModels() {
    for (Model* m : models) {
        delete m;
    }
    models.clear();

    models = ModelFactory::createDefaultModels();
}

void Application::createShaders() {
    for (ShaderProgram* sp : shaders) {
        delete sp;
    }
    shaders.clear();

    ShaderBundle bundle = ShaderFactory::createDefaultShaders();
    shaders = std::move(bundle.allShaders);

    clockShader = bundle.clockShader;
    treeShader = bundle.treeShader;
    phongShader = bundle.phongShader;
    blinnShader = bundle.blinnShader;
    ambientShader = bundle.ambientShader;
    blinnColorShader = bundle.blinnColorShader;
    nightShader = bundle.nightShader;
    textureShader = bundle.textureShader;
    skyboxShader = bundle.skyboxShader;

    for (ShaderProgram* sp : shaders) {
        if (camera && sp) {
            camera->attach(sp);
        }
    }
}

void Application::createScenes() {
    for (Scene* s : scenes) {
        delete s;
    }
    scenes.clear();

    SceneBuildContext ctx{
        models,
        shaders,
        clockShader,
        blinnShader,
        phongShader,
        blinnColorShader,
        nightShader,
        textureShader
    };

    SceneBuildResult res = SceneFactory::createDefaultScenes(ctx);

    scenes = std::move(res.scenes);
    animRefs = {
            res.triRotLocal, res.triRotOrbit,
            res.sunSelfRotation, res.earthOrbitRotation, res.earthSelfRotation,
            res.moonOrbitRotation, res.moonSelfRotation,
            res.rocketOrbitRotation, res.rocketSelfRotation
    };

    skyboxTex = res.skyboxTex;
    shrekTex = res.shrekTex;
    tableTex = res.tableTex;
    ocTex = res.ocTex;
    shrekebTex = res.shrekebTex;
}

void Application::run() {
    while (!glfwWindowShouldClose(window)) {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        inputController.processContinuousInput(window, camera, deltaTime);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        animationController.update(currentFrame, animRefs, clockShader);
        lightController.update(activeSceneIndex, nightShader, camera);
        skyboxRenderer.render(activeSceneIndex, skyboxShader, skyboxTex, models[15]);

        movingController.update(activeSceneIndex, deltaTime);

        if (activeSceneIndex < scenes.size() && scenes[activeSceneIndex] != nullptr) {
            scenes[activeSceneIndex]->render();
        }

        scenePicker.update(
            window, camera, scenes[activeSceneIndex], activeSceneIndex,
            models, shaders, nightShader, textureShader,
            shrekTex, tableTex, ocTex, movingController
        );

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}