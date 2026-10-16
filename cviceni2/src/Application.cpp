/**
 * @file Application.cpp
 * @author Student (login: ZHA0067)
 * @brief Application lifecycle coordinator with static scenes and scene switcher.
 */
#include "Application.h"
#include <iostream>
#include <cstdlib>
#include <glm/gtc/matrix_transform.hpp>
#include <ctime>

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
     0.0f,  0.55f, 0.0f,   1.0f, 0.0f, 0.0f,
    -0.55f, -0.45f, 0.0f,  0.0f, 1.0f, 0.0f,
     0.55f, -0.45f, 0.0f,  0.0f, 0.0f, 1.0f
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
    Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
    if (app) {
        app->windowWidth = width;
        app->windowHeight = height;
    }
}

Application::Application()
    : window(nullptr), windowWidth(1024), windowHeight(768), activeSceneIndex(0) {}

Application::~Application() {
    for (Scene* s : scenes) delete s;
    scenes.clear();

    for (Model* m : models) delete m;
    models.clear();

    for (ShaderProgram* sp : shaders) delete sp;
    shaders.clear();

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

    window = glfwCreateWindow(windowWidth, windowHeight, "ZPG (ZHA0067)", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetWindowUserPointer(window, this);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    glfwSetKeyCallback(window, key_callback);
    glfwSetWindowSizeCallback(window, window_size_callback);

    if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        exit(EXIT_FAILURE);
    }
    
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL Version:   " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

    glEnable(GL_DEPTH_TEST);
}

void Application::createShaders() {
    for (ShaderProgram* sp : shaders) {
        delete sp;
    }
    shaders.clear();

    Shader vertColor(GL_VERTEX_SHADER, "shaders/vertex_color.vert");
    Shader fragColor(GL_FRAGMENT_SHADER, "shaders/vertex_color.frag");
    shaders.push_back(new ShaderProgram(vertColor, fragColor));

    Shader vertNormal(GL_VERTEX_SHADER, "shaders/transform.vert");
    Shader fragAbsNormal(GL_FRAGMENT_SHADER, "shaders/abs_normals.frag");
    shaders.push_back(new ShaderProgram(vertNormal, fragAbsNormal));

    Shader fragUniformColor(GL_FRAGMENT_SHADER, "shaders/color_uniform.frag");
    shaders.push_back(new ShaderProgram(vertNormal, fragUniformColor));
}


void Application::createModels() {
    models.push_back(new Model(triangleVertices, sizeof(triangleVertices), 3));
    models.push_back(new Model(sphere, sizeof(sphere), 2880));
    models.push_back(new Model(plain, sizeof(plain), 6));
    models.push_back(new Model(tree, sizeof(tree), 92814));
    models.push_back(new Model(bushes, sizeof(bushes), 8730));
    models.push_back(new Model(gift, sizeof(gift), 66624));
    models.push_back(new Model(zha0067, sizeof(zha0067), 1776));

    models.push_back(new Model(sun, sizeof(sun), 11520, true));
    models.push_back(new Model(earth, sizeof(earth), 11520, true));
    models.push_back(new Model(moon, sizeof(moon), 11520, true));
}

void Application::addWatermark(Scene* scene) {
    auto wmComp = std::make_shared<CompositeTransformation>();
    std::string name = scene->getName();

    if (name.find("Solar") != std::string::npos || name.find("SolarSystem") != std::string::npos) {
        wmComp->add(std::make_shared<Rotation>(glm::radians(-25.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
        wmComp->add(std::make_shared<Translation>(11.5f, -7.5f, 3.5f));
        wmComp->add(std::make_shared<Scale>(1.55f, 1.55f, 0.001f));
    } else if (name.find("Forest") != std::string::npos || name.find("Woodland") != std::string::npos) {
        wmComp->add(std::make_shared<Rotation>(glm::radians(-18.5f), glm::vec3(1.0f, 0.0f, 0.0f)));
        wmComp->add(std::make_shared<Translation>(-4.0f, 3.5f, 0.0f));
        wmComp->add(std::make_shared<Scale>(0.6f, 0.6f, 0.001f));
    } else {
        wmComp->add(std::make_shared<Translation>(1.05f, -0.75f, 0.0f));
        wmComp->add(std::make_shared<Scale>(0.18f, 0.18f, 0.001f));
    }

    scene->addObject(new DrawableObject(models[6], shaders[2], wmComp, glm::vec3(0.0f, 0.0f, 1.0f)));
}

void Application::createScenes() {
    Scene* scene1 = new Scene("1: Triangle");

    auto comp1 = std::make_shared<CompositeTransformation>();
    comp1->add(std::make_shared<Translation>(-0.85f, 0.0f, 0.0f));
    triRotLocal = std::make_shared<Rotation>(0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
    comp1->add(triRotLocal);
    comp1->add(std::make_shared<Scale>(0.65f));
    scene1->addObject(new DrawableObject(models[0], shaders[0], comp1));

    auto comp2 = std::make_shared<CompositeTransformation>();
    triRotOrbit = std::make_shared<Rotation>(0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
    comp2->add(triRotOrbit);
    comp2->add(std::make_shared<Translation>(0.85f, 0.0f, 0.0f));
    comp2->add(std::make_shared<Scale>(0.65f));
    scene1->addObject(new DrawableObject(models[0], shaders[0], comp2));

    addWatermark(scene1);
    scenes.push_back(scene1);
    
    Scene* scene2 = new Scene("2: Sphere");
    auto sphereComp = std::make_shared<CompositeTransformation>();
    sphereComp->add(std::make_shared<Scale>(0.85f));
    scene2->addObject(new DrawableObject(models[1], shaders[1], sphereComp));
    addWatermark(scene2);
    scenes.push_back(scene2);

    Scene* scene3 = new Scene("3: Forest");

    auto plainComp = std::make_shared<CompositeTransformation>();
    plainComp->add(std::make_shared<Translation>(0.0f, -0.65f, 0.0f));
    plainComp->add(std::make_shared<Scale>(7.5f, 1.0f, 7.5f));
    scene3->addObject(new DrawableObject(models[2], shaders[2], plainComp, glm::vec3(0.22f, 0.52f, 0.22f)));

    auto forestSunComp = std::make_shared<CompositeTransformation>();
    forestSunComp->add(std::make_shared<Translation>(3.8f, 3.2f, -2.5f));
    forestSunComp->add(std::make_shared<Scale>(0.75f));
    scene3->addObject(new DrawableObject(models[1], shaders[2], forestSunComp, glm::vec3(1.0f, 0.95f, 0.15f)));

    auto getRandomFloat = [](float minVal, float maxVal) -> float {
        return minVal + static_cast<float>(std::rand()) / (static_cast<float>(RAND_MAX / (maxVal - minVal)));
    };

    const int treeCount = 55;
    for (int i = 0; i < treeCount; ++i) {
        float posX = getRandomFloat(-3.4f, 3.4f);
        float posZ = getRandomFloat(-3.4f, 2.5f);
        float posY = -0.60f;

        float rotAngle = getRandomFloat(0.0f, 360.0f);

        float scaleVal = getRandomFloat(0.22f, 0.42f);

        auto treeComp = std::make_shared<CompositeTransformation>();
        treeComp->add(std::make_shared<Translation>(posX, posY, posZ));
        treeComp->add(std::make_shared<Rotation>(glm::radians(rotAngle), glm::vec3(0.0f, 1.0f, 0.0f)));
        treeComp->add(std::make_shared<Scale>(scaleVal));

        float greenTone = getRandomFloat(0.42f, 0.58f);
        glm::vec3 treeColor(0.12f, greenTone, 0.14f);

        scene3->addObject(new DrawableObject(models[3], shaders[2], treeComp, treeColor));
    }

    const int bushCount = 35;
    for (int i = 0; i < bushCount; ++i) {
        float posX = getRandomFloat(-3.3f, 3.3f);
        float posZ = getRandomFloat(-3.3f, 3.0f);
        float posY = -0.56f;

        float rotAngle = getRandomFloat(0.0f, 360.0f);
        float scaleVal = getRandomFloat(0.12f, 0.22f);

        auto bushComp = std::make_shared<CompositeTransformation>();
        bushComp->add(std::make_shared<Translation>(posX, posY, posZ));
        bushComp->add(std::make_shared<Rotation>(glm::radians(rotAngle), glm::vec3(0.0f, 1.0f, 0.0f)));
        bushComp->add(std::make_shared<Scale>(scaleVal));

        float greenTone = getRandomFloat(0.55f, 0.70f);
        glm::vec3 bushColor(0.16f, greenTone, 0.20f);

        scene3->addObject(new DrawableObject(models[4], shaders[2], bushComp, bushColor));
    }

    addWatermark(scene3);
    scenes.push_back(scene3);

    Scene* scene4 = new Scene("4: Model ZHA0067");
    auto loginComp = std::make_shared<CompositeTransformation>();
    
    loginComp->add(std::make_shared<Scale>(0.85f));
    
    scene4->addObject(new DrawableObject(models[6], shaders[1], loginComp));
    scenes.push_back(scene4);

    Scene* scene5 = new Scene("5: Solar System");

    auto sunComp = std::make_shared<CompositeTransformation>();
    sunSelfRotation = std::make_shared<Rotation>(0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    sunComp->add(sunSelfRotation);
    sunComp->add(std::make_shared<Scale>(1.6f));
    scene5->addObject(new DrawableObject(models[7], shaders[0], sunComp));

    auto earthComp = std::make_shared<CompositeTransformation>();
    earthOrbitRotation = std::make_shared<Rotation>(0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    earthSelfRotation = std::make_shared<Rotation>(0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    earthComp->add(earthOrbitRotation);
    earthComp->add(std::make_shared<Translation>(7.0f, 0.0f, 0.0f));
    earthComp->add(earthSelfRotation);
    earthComp->add(std::make_shared<Scale>(0.6f));
    scene5->addObject(new DrawableObject(models[8], shaders[0], earthComp));

    auto moonComp = std::make_shared<CompositeTransformation>();
    moonOrbitRotation = std::make_shared<Rotation>(0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    moonSelfRotation = std::make_shared<Rotation>(0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    moonComp->add(earthOrbitRotation);
    moonComp->add(std::make_shared<Translation>(7.0f, 0.0f, 0.0f));
    moonComp->add(moonOrbitRotation);
    moonComp->add(std::make_shared<Translation>(1.5f, 0.0f, 0.0f));
    moonComp->add(moonSelfRotation);
    moonComp->add(std::make_shared<Scale>(0.22f));
    scene5->addObject(new DrawableObject(models[9], shaders[0], moonComp));

    addWatermark(scene5);
    scenes.push_back(scene5);
}

void Application::run() {
    glEnable(GL_DEPTH_TEST);


    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float time = static_cast<float>(glfwGetTime());
        if (triRotLocal) triRotLocal->setAngle(time * 1.5f);
        if (triRotOrbit) triRotOrbit->setAngle(time * 1.5f);

        if (sunSelfRotation)    sunSelfRotation->setAngle(time * 0.35f);
        if (earthOrbitRotation) earthOrbitRotation->setAngle(time * 0.55f);
        if (earthSelfRotation)  earthSelfRotation->setAngle(time * 2.2f);
        if (moonOrbitRotation)  moonOrbitRotation->setAngle(time * 3.2f);
        if (moonSelfRotation)   moonSelfRotation->setAngle(time * 1.5f);

        float aspect = (windowHeight > 0) ? (static_cast<float>(windowWidth) / static_cast<float>(windowHeight)) : (4.0f / 3.0f);
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 500.0f);

        glm::mat4 view = glm::mat4(1.0f);

        if (activeSceneIndex == 4) {
            view = glm::lookAt(
                glm::vec3(0.0f, 12.0f, 25.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 1.0f, 0.0f)
            );
        } else if (activeSceneIndex == 2) {
            view = glm::lookAt(
                glm::vec3(0.0f, 3.5f, 9.5f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 1.0f, 0.0f)
            );
        } else if (activeSceneIndex == 3) {
            view = glm::lookAt(
                glm::vec3(0.0f, 0.0f, 2.5f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 1.0f, 0.0f)
            );
        } else {
            view = glm::lookAt(
                glm::vec3(0.0f, 0.0f, 2.5f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 1.0f, 0.0f)
            );
        }

        for (ShaderProgram* sp : shaders) {
            sp->use();
            sp->setUniform("viewMatrix", view);
            sp->setUniform("projectMatrix", projection);
        }

        if (activeSceneIndex < scenes.size() && scenes[activeSceneIndex] != nullptr) {
            scenes[activeSceneIndex]->render();
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void Application::handleKeyboard(int key, int action) {
    if (action == GLFW_PRESS) {
        if (key == GLFW_KEY_ESCAPE) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }
        else if (key >= GLFW_KEY_1 && key <= GLFW_KEY_5) {
            size_t idx = static_cast<size_t>(key - GLFW_KEY_1);
            if (idx < scenes.size()) {
                activeSceneIndex = idx;
                std::cout << "Switched to scene: " << scenes[idx]->getName() << std::endl;
            }
        }
    }
}