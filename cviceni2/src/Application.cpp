/**
 * @file Application.cpp
 * @author Student (login: ZHA0067)
 * @brief Application lifecycle coordinator with static scenes and scene switcher.
 */
#include "Application.h"
#include <iostream>
#include <cstdlib>
#include <glm/gtc/matrix_transform.hpp>

#include "../models/sun.h"
#include "../models/earth.h"
#include "../models/moon.h"
#include "../models/sphere.h"
#include "../models/plain.h"
#include "../models/tree.h"
#include "../models/bushes.h"
#include "../models/gift.h"
#include "../models/zha0067.h"

static const float triangleVertices[] = {
     0.0f,  0.5f,  0.0f,   0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f,  0.0f,   0.0f, 0.0f, 1.0f,
     0.5f, -0.5f,  0.0f,   0.0f, 0.0f, 1.0f
};

void Application::error_callback(int error, const char* description) {
    std::cerr << "GLFW Error [" << error << "]: " << description << std::endl;
}

void Application::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
    if (app) {
        app->handleKeyboard(key, action);
    }
}

void Application::window_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

Application::Application()
    : window(nullptr), windowWidth(800), windowHeight(600), activeSceneIndex(0),
    sunObject(nullptr), earthObject(nullptr), moonObject(nullptr), marsObject(nullptr) {}

Application::~Application() {
    for (Scene* s : scenes) {
        delete s;
    }
    scenes.clear();

    for (Model* m : models) {
        delete m;
    }
    models.clear();

    for (ShaderProgram* sp : shaders) {
        delete sp;
    }
    shaders.clear();

    if (window) {
        glfwDestroyWindow(window);
    }
    glfwTerminate();
}

void Application::initialization() {
    glfwSetErrorCallback(Application::error_callback);

    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW!" << std::endl;
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(windowWidth, windowHeight, "ZPG: Solar System & Transformations", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window!" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable V-Sync

    glfwSetWindowUserPointer(window, this);
    glfwSetKeyCallback(window, Application::key_callback);
    glfwSetWindowSizeCallback(window, Application::window_size_callback);

    if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD!" << std::endl;
        exit(EXIT_FAILURE);
    }

    glEnable(GL_DEPTH_TEST);

    glClearColor(0.05f, 0.05f, 0.08f, 1.0f);

    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "Controls:\n"
        << "  [1] - Scene 1: Solar System (Planetary Hierarchy)\n"
        << "  [2] - Scene 2: Forest Landscape (Distant Panoramic View)\n"
        << "  [3] - Scene 3: Sphere (Abs Normals - Close-up)\n"
        << "  [4] - Scene 4: Student Login (ZHA0067 - Large)\n"
        << "  [ESC] - Exit\n" << std::endl;
}

void Application::createShaders() {
    Shader vertShader(GL_VERTEX_SHADER, "shaders/transform.vert");

    Shader fragAbs(GL_FRAGMENT_SHADER, "shaders/abs_normals.frag");
    shaders.push_back(new ShaderProgram(vertShader, fragAbs));

    Shader fragColor(GL_FRAGMENT_SHADER, "shaders/color_uniform.frag");
    shaders.push_back(new ShaderProgram(vertShader, fragColor));
}

void Application::createModels() {
    models.push_back(new Model(triangleVertices, sizeof(triangleVertices), 3));

    models.push_back(new Model(sphere, sizeof(sphere), 2880));

    GLsizei treeCount = sizeof(tree) / (6 * sizeof(float));
    models.push_back(new Model(tree, sizeof(tree), treeCount));

    models.push_back(new Model(bushes, sizeof(bushes), 8730));

    models.push_back(new Model(plain, sizeof(plain), 6));

    models.push_back(new Model(gift, sizeof(gift), 66624));

    GLsizei loginCount = sizeof(zha0067) / (6 * sizeof(float));
    models.push_back(new Model(zha0067, sizeof(zha0067), loginCount));
}

void Application::createScenes() {
    ShaderProgram* spAbs = shaders[0];
    ShaderProgram* spColor = shaders[1];

    Model* mSphere = models[1];
    Model* mTree = models[2];
    Model* mBushes = models[3];
    Model* mPlain = models[4];
    Model* mLogin = models[6];

    Scene* sceneSolar = new Scene("Scene 1: Solar System");

    sunObject = new DrawableObject(mSphere, spColor, Transformation(), glm::vec3(1.0f, 0.85f, 0.1f));
    sceneSolar->addObject(sunObject);

    earthObject = new DrawableObject(mSphere, spColor, Transformation(), glm::vec3(0.2f, 0.55f, 1.0f));
    sceneSolar->addObject(earthObject);

    moonObject = new DrawableObject(mSphere, spColor, Transformation(), glm::vec3(0.85f, 0.85f, 0.85f));
    sceneSolar->addObject(moonObject);

    marsObject = new DrawableObject(mSphere, spColor, Transformation(), glm::vec3(0.95f, 0.35f, 0.15f));
    sceneSolar->addObject(marsObject);

    Transformation solarSignTrans;
    solarSignTrans.translate(glm::vec3(4.5f, -3.2f, 2.0f));
    solarSignTrans.scale(glm::vec3(0.55f));
    sceneSolar->addObject(new DrawableObject(mLogin, spColor, solarSignTrans, glm::vec3(0.2f, 0.85f, 1.0f)));

    scenes.push_back(sceneSolar);

    Scene* sceneForest = new Scene("Scene 2: Forest");

    Transformation plainTrans;
    plainTrans.translate(glm::vec3(0.0f, -0.2f, 0.0f));
    plainTrans.scale(glm::vec3(12.0f, 1.0f, 12.0f));
    sceneForest->addObject(new DrawableObject(mPlain, spColor, plainTrans, glm::vec3(0.18f, 0.48f, 0.18f)));

    Transformation forestSunTrans;
    forestSunTrans.translate(glm::vec3(4.5f, 5.2f, -5.5f));
    forestSunTrans.scale(glm::vec3(1.6f));
    sceneForest->addObject(new DrawableObject(mSphere, spColor, forestSunTrans, glm::vec3(1.0f, 0.95f, 0.2f)));

    float treeCoords[12][3] = {
        {-4.5f, -0.2f, -3.0f}, {-2.2f, -0.2f, -4.5f}, {-0.8f, -0.2f, -2.5f},
        { 1.8f, -0.2f, -3.8f}, { 3.8f, -0.2f, -3.0f}, { 5.2f, -0.2f, -4.8f},
        {-5.2f, -0.2f, -1.0f}, {-3.2f, -0.2f, -1.8f}, {-1.5f, -0.2f, -5.5f},
        { 0.8f, -0.2f, -1.8f}, { 2.8f, -0.2f, -2.2f}, { 4.6f, -0.2f, -0.8f}
    };
    for (int i = 0; i < 12; ++i) {
        Transformation t;
        t.translate(glm::vec3(treeCoords[i][0], treeCoords[i][1], treeCoords[i][2]));
        t.rotate(i * 0.7f, glm::vec3(0.0f, 1.0f, 0.0f));
        t.scale(glm::vec3(0.7f + (i % 3) * 0.15f));
        sceneForest->addObject(new DrawableObject(mTree, spColor, t, glm::vec3(0.12f, 0.58f, 0.18f)));
    }

    float bushCoords[12][3] = {
        {-3.5f, -0.1f, 0.8f}, {-2.0f, -0.1f, 0.0f}, {-0.8f, -0.1f, 1.0f},
        { 1.0f, -0.1f, 0.5f}, { 2.5f, -0.1f, 0.8f}, { 4.0f, -0.1f, 0.0f},
        {-4.2f, -0.1f, 1.5f}, {-2.8f, -0.1f, 1.8f}, {-1.5f, -0.1f, 2.0f},
        { 0.4f, -0.1f, 1.6f}, { 1.8f, -0.1f, 2.0f}, { 3.2f, -0.1f, 1.5f}
    };
    for (int i = 0; i < 12; ++i) {
        Transformation t;
        t.translate(glm::vec3(bushCoords[i][0], bushCoords[i][1], bushCoords[i][2]));
        t.scale(glm::vec3(1.1f + (i % 4) * 0.15f));
        sceneForest->addObject(new DrawableObject(mBushes, spColor, t, glm::vec3(0.22f, 0.75f, 0.28f)));
    }

    Transformation forestSignTrans;
    forestSignTrans.translate(glm::vec3(4.2f, 0.1f, 3.5f));
    forestSignTrans.scale(glm::vec3(0.55f));
    sceneForest->addObject(new DrawableObject(mLogin, spColor, forestSignTrans, glm::vec3(0.2f, 0.85f, 1.0f)));

    scenes.push_back(sceneForest);

    Scene* sceneSphere = new Scene("Scene 3: Sphere");

    Transformation sphereTrans;
    sphereTrans.scale(glm::vec3(1.0f));
    sceneSphere->addObject(new DrawableObject(mSphere, spAbs, sphereTrans));

    Transformation sphereSignTrans;
    sphereSignTrans.translate(glm::vec3(1.15f, -0.95f, 0.0f));
    sphereSignTrans.scale(glm::vec3(0.18f));
    sceneSphere->addObject(new DrawableObject(mLogin, spColor, sphereSignTrans, glm::vec3(0.2f, 0.85f, 1.0f)));

    scenes.push_back(sceneSphere);

    Scene* sceneLogin = new Scene("Scene 4: Student Login");

    Transformation loginTrans;
    loginTrans.translate(glm::vec3(0.0f, 0.0f, 0.0f));
    loginTrans.scale(glm::vec3(1.0f));
    sceneLogin->addObject(new DrawableObject(mLogin, spAbs, loginTrans));

    scenes.push_back(sceneLogin);
}

void Application::updateSolarSystem(float currentTime) {
    if (!sunObject || !earthObject || !moonObject || !marsObject) return;

    glm::mat4 sunMatrix = glm::mat4(1.0f);
    sunMatrix = glm::rotate(sunMatrix, currentTime * 0.3f, glm::vec3(0.0f, 1.0f, 0.0f));
    sunMatrix = glm::scale(sunMatrix, glm::vec3(1.6f));
    sunObject->setTransformation(Transformation(sunMatrix));

    float earthOrbitRadius = 5.2f;
    glm::mat4 earthOrbit = glm::rotate(glm::mat4(1.0f), currentTime * 0.8f, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 earthCenterMatrix = glm::translate(earthOrbit, glm::vec3(earthOrbitRadius, 0.0f, 0.0f));

    glm::mat4 earthMatrix = glm::rotate(earthCenterMatrix, currentTime * 3.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    earthMatrix = glm::scale(earthMatrix, glm::vec3(0.75f));
    earthObject->setTransformation(Transformation(earthMatrix));

    float moonOrbitRadius = 1.4f;
    glm::mat4 moonMatrix = glm::rotate(earthCenterMatrix, currentTime * 4.0f, glm::vec3(0.0f, 1.0f, 0.2f));
    moonMatrix = glm::translate(moonMatrix, glm::vec3(moonOrbitRadius, 0.0f, 0.0f));
    moonMatrix = glm::scale(moonMatrix, glm::vec3(0.3f));
    moonObject->setTransformation(Transformation(moonMatrix));

    float marsOrbitRadius = 7.8f;
    glm::mat4 marsMatrix = glm::rotate(glm::mat4(1.0f), currentTime * 0.45f, glm::vec3(0.0f, 1.0f, 0.0f));
    marsMatrix = glm::translate(marsMatrix, glm::vec3(marsOrbitRadius, 0.0f, 0.0f));
    marsMatrix = glm::rotate(marsMatrix, currentTime * 2.5f, glm::vec3(0.0f, 1.0f, 0.0f));
    marsMatrix = glm::scale(marsMatrix, glm::vec3(0.6f));
    marsObject->setTransformation(Transformation(marsMatrix));
}

void Application::handleKeyboard(int key, int action) {
    if (action != GLFW_PRESS) return;

    if (key == GLFW_KEY_ESCAPE) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
        return;
    }

    if (key == GLFW_KEY_1 && scenes.size() > 0) {
        activeSceneIndex = 0;
        std::cout << "Switched to: " << scenes[0]->getName() << std::endl;
    }
    else if (key == GLFW_KEY_2 && scenes.size() > 1) {
        activeSceneIndex = 1;
        std::cout << "Switched to: " << scenes[1]->getName() << std::endl;
    }
    else if (key == GLFW_KEY_3 && scenes.size() > 2) {
        activeSceneIndex = 2;
        std::cout << "Switched to: " << scenes[2]->getName() << std::endl;
    }
    else if (key == GLFW_KEY_4 && scenes.size() > 3) {
        activeSceneIndex = 3;
        std::cout << "Switched to: " << scenes[3]->getName() << std::endl;
    }
}

void Application::run() {
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float currentTime = static_cast<float>(glfwGetTime());
        updateSolarSystem(currentTime);

        glm::mat4 projection = glm::perspective(
            glm::radians(45.0f),
            static_cast<float>(windowWidth) / static_cast<float>(windowHeight),
            0.1f,
            100.0f
        );

        glm::mat4 view = glm::mat4(1.0f);

        if (activeSceneIndex == 0) {
            view = glm::lookAt(
                glm::vec3(0.0f, 9.0f, 12.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 1.0f, 0.0f)
            );
        }
        else if (activeSceneIndex == 1) {
            view = glm::lookAt(
                glm::vec3(0.0f, 3.2f, 9.5f),
                glm::vec3(0.0f, 1.0f, -1.5f),
                glm::vec3(0.0f, 1.0f, 0.0f)
            );
        }
        else if (activeSceneIndex == 2) {
            view = glm::lookAt(
                glm::vec3(0.0f, 0.0f, 2.7f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 1.0f, 0.0f)
            );
        }
        else if (activeSceneIndex == 3) {
            view = glm::lookAt(
                glm::vec3(0.0f, 0.0f, 2.8f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 1.0f, 0.0f)
            );
        }

        if (activeSceneIndex < scenes.size() && scenes[activeSceneIndex] != nullptr) {
            scenes[activeSceneIndex]->setMatrices(view, projection);
            scenes[activeSceneIndex]->render();
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}