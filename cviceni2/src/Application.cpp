/**
 * @file Application.cpp
 * @author Student (login: ZHA0067)
 * @brief Application lifecycle coordinator with static scenes and scene switcher.
 */
#include "Application.h"
#include <iostream>
#include <cstdlib>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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
    -0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,
     0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,
     0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f
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
    : window(nullptr), windowWidth(800), windowHeight(600), activeSceneIndex(4) {}

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
    glfwSetErrorCallback(error_callback);

    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(windowWidth, windowHeight, "ZPG Project - Transformations (ZHA0067)", nullptr, nullptr);
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

    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "Vendor:         " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer:       " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "GLSL Version:   " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

    glEnable(GL_DEPTH_TEST);
}

void Application::createShaders() {
    Shader basicVert(GL_VERTEX_SHADER, "shaders/basic.vert");
    Shader basicFrag(GL_FRAGMENT_SHADER, "shaders/basic.frag");
    shaders.push_back(new ShaderProgram(basicVert, basicFrag));

    Shader transVert(GL_VERTEX_SHADER, "shaders/transform.vert");
    Shader absNormFrag(GL_FRAGMENT_SHADER, "shaders/abs_normals.frag");
    shaders.push_back(new ShaderProgram(transVert, absNormFrag));

    Shader colorFrag(GL_FRAGMENT_SHADER, "shaders/color_uniform.frag");
    shaders.push_back(new ShaderProgram(transVert, colorFrag));

    Shader planetVert(GL_VERTEX_SHADER, "shaders/planet.vert");
    Shader planetFrag(GL_FRAGMENT_SHADER, "shaders/planet.frag");
    shaders.push_back(new ShaderProgram(planetVert, planetFrag));
}

void Application::createModels() {
    models.push_back(new Model(triangleVertices, sizeof(triangleVertices), 3));
    models.push_back(new Model(sphere, sizeof(sphere), 2880));
    models.push_back(new Model(plain, sizeof(plain), 6));
    models.push_back(new Model(tree, sizeof(tree), 92814));
    models.push_back(new Model(bushes, sizeof(bushes), 8730));
    models.push_back(new Model(gift, sizeof(gift), 66624));
    models.push_back(new Model(zha0067, sizeof(zha0067), 1776));

    models.push_back(new Model(sun, sizeof(sun), static_cast<GLsizei>(sizeof(sun) / (6 * sizeof(float)))));
    models.push_back(new Model(earth, sizeof(earth), static_cast<GLsizei>(sizeof(earth) / (6 * sizeof(float)))));
    models.push_back(new Model(moon, sizeof(moon), static_cast<GLsizei>(sizeof(moon) / (6 * sizeof(float)))));
}

void Application::addWatermark(Scene* scene) {
    if (!scene) return;

    auto watermarkComposite = std::make_shared<CompositeTransformation>();
    watermarkComposite->add(std::make_shared<Translation>(2.2f, 1.8f, -1.0f));
    watermarkComposite->add(std::make_shared<Scale>(0.25f));

    DrawableObject* watermarkObj = new DrawableObject(models[6], shaders[1], watermarkComposite);
    watermarkObj->setWatermark(true);
    scene->addObject(watermarkObj);
}

void Application::createScenes() {
    Scene* scene1 = new Scene("Triangle - Matrix Composite");
    auto triComp = std::make_shared<CompositeTransformation>();
    triComp->add(std::make_shared<Translation>(0.0f, 0.0f, 0.0f));
    triComp->add(std::make_shared<Rotation>(glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
    triComp->add(std::make_shared<Scale>(1.2f));
    scene1->addObject(new DrawableObject(models[0], shaders[0], triComp));
    addWatermark(scene1);
    scenes.push_back(scene1);

    Scene* scene2 = new Scene("Sphere - Normal Shading");
    auto sphereComp = std::make_shared<CompositeTransformation>();
    sphereComp->add(std::make_shared<Scale>(1.0f));
    scene2->addObject(new DrawableObject(models[1], shaders[1], sphereComp));
    addWatermark(scene2);
    scenes.push_back(scene2);

    Scene* scene3 = new Scene("Forest Landscape");

    auto plainComp = std::make_shared<CompositeTransformation>();
    plainComp->add(std::make_shared<Translation>(0.0f, -0.6f, 0.0f));
    plainComp->add(std::make_shared<Scale>(3.0f));
    scene3->addObject(new DrawableObject(models[2], shaders[2], plainComp, glm::vec3(0.2f, 0.5f, 0.2f)));

    auto forestSunComp = std::make_shared<CompositeTransformation>();
    forestSunComp->add(std::make_shared<Translation>(1.5f, 2.0f, -1.0f));
    forestSunComp->add(std::make_shared<Scale>(0.35f));
    scene3->addObject(new DrawableObject(models[1], shaders[2], forestSunComp, glm::vec3(1.0f, 0.9f, 0.1f)));

    auto giftComp = std::make_shared<CompositeTransformation>();
    giftComp->add(std::make_shared<Translation>(0.0f, -0.4f, 0.5f));
    giftComp->add(std::make_shared<Scale>(0.5f));
    scene3->addObject(new DrawableObject(models[5], shaders[2], giftComp, glm::vec3(0.85f, 0.15f, 0.15f)));

    float treeX[] = { -2.2f, -1.8f, -1.4f, -1.0f, -0.6f, -0.2f, 0.2f, 0.6f, 1.0f, 1.4f, 1.8f, 2.2f, -0.9f, 0.9f };
    for (int i = 0; i < 14; i++) {
        auto treeComp = std::make_shared<CompositeTransformation>();
        float z = (i % 2 == 0) ? -1.2f : -0.6f;
        float s = (i % 2 == 0) ? 0.35f : 0.45f;
        treeComp->add(std::make_shared<Translation>(treeX[i], -0.6f, z));
        treeComp->add(std::make_shared<Rotation>(glm::radians(i * 25.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
        treeComp->add(std::make_shared<Scale>(s));
        scene3->addObject(new DrawableObject(models[3], shaders[2], treeComp, glm::vec3(0.13f, 0.55f, 0.13f)));
    }

    float bushX[] = { -2.0f, -1.6f, -1.2f, -0.8f, -0.4f, 0.0f, 0.4f, 0.8f, 1.2f, 1.6f, 2.0f, -0.2f };
    for (int i = 0; i < 12; i++) {
        auto bushComp = std::make_shared<CompositeTransformation>();
        bushComp->add(std::make_shared<Translation>(bushX[i], -0.6f, 0.2f));
        bushComp->add(std::make_shared<Scale>(0.35f));
        scene3->addObject(new DrawableObject(models[4], shaders[2], bushComp, glm::vec3(0.18f, 0.65f, 0.22f)));
    }

    addWatermark(scene3);
    scenes.push_back(scene3);

    Scene* scene4 = new Scene("Student Model: ZHA0067");
    auto loginComp = std::make_shared<CompositeTransformation>();
    loginComp->add(std::make_shared<Scale>(1.0f));
    scene4->addObject(new DrawableObject(models[6], shaders[1], loginComp));
    scenes.push_back(scene4);

    Scene* solarScene = new Scene("Solar System (Slunecni soustava)");
    ShaderProgram* planetShader = shaders[3];

    auto sunComp = std::make_shared<CompositeTransformation>();
    sunSelfRotation = std::make_shared<Rotation>(0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    auto sunScale = std::make_shared<Scale>(0.6f);
    sunComp->add(sunSelfRotation);
    sunComp->add(sunScale);
    solarScene->addObject(new DrawableObject(models[7], planetShader, sunComp));

    auto earthComp = std::make_shared<CompositeTransformation>();
    earthOrbitRotation = std::make_shared<Rotation>(0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    auto earthOrbitTranslate = std::make_shared<Translation>(2.4f, 0.0f, 0.0f);
    earthSelfRotation = std::make_shared<Rotation>(0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    auto earthScale = std::make_shared<Scale>(0.28f);

    earthComp->add(earthOrbitRotation);
    earthComp->add(earthOrbitTranslate);
    earthComp->add(earthSelfRotation);
    earthComp->add(earthScale);
    solarScene->addObject(new DrawableObject(models[8], planetShader, earthComp));

    auto moonComp = std::make_shared<CompositeTransformation>();
    moonOrbitRotation = std::make_shared<Rotation>(0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    auto moonOrbitTranslate = std::make_shared<Translation>(0.6f, 0.0f, 0.0f);
    moonSelfRotation = std::make_shared<Rotation>(0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    auto moonScale = std::make_shared<Scale>(0.1f);

    moonComp->add(earthOrbitRotation);
    moonComp->add(earthOrbitTranslate);
    moonComp->add(moonOrbitRotation);
    moonComp->add(moonOrbitTranslate);
    moonComp->add(moonSelfRotation);
    moonComp->add(moonScale);
    solarScene->addObject(new DrawableObject(models[9], planetShader, moonComp));

    addWatermark(solarScene);
    scenes.push_back(solarScene);
}

void Application::run() {
    glEnable(GL_DEPTH_TEST);

    std::cout << " 1: Trojuhelnik (Skladani transformaci)\n";
    std::cout << " 2: Koule (Stinovani normal)\n";
    std::cout << " 3: Les (Krajina se stromy a keri)\n";
    std::cout << " 4: Model studenta (ZHA0067)\n";
    std::cout << " 5: Slunecni soustava (Slunce, Zeme, Mesic)\n";
    std::cout << "Aktualni scena: 5 (Slunecni soustava)\n";

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float time = static_cast<float>(glfwGetTime());

        if (sunSelfRotation)    sunSelfRotation->setAngle(time * 0.4f);
        if (earthOrbitRotation) earthOrbitRotation->setAngle(time * 0.7f);
        if (earthSelfRotation)  earthSelfRotation->setAngle(time * 2.5f);
        if (moonOrbitRotation)  moonOrbitRotation->setAngle(time * 3.8f);
        if (moonSelfRotation)   moonSelfRotation->setAngle(time * 1.5f);

        glm::mat4 view = glm::lookAt(
            glm::vec3(0.0f, 3.8f, 5.5f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f)
        );

        float aspectRatio = (windowHeight > 0) ? (static_cast<float>(windowWidth) / static_cast<float>(windowHeight)) : (800.0f / 600.0f);
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);

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
        else if (key == GLFW_KEY_1 && scenes.size() > 0) {
            activeSceneIndex = 0;
            std::cout << "Prepnuto na scenu: 1 (" << scenes[0]->getName() << ")" << std::endl;
        }
        else if (key == GLFW_KEY_2 && scenes.size() > 1) {
            activeSceneIndex = 1;
            std::cout << "Prepnuto na scenu: 2 (" << scenes[1]->getName() << ")" << std::endl;
        }
        else if (key == GLFW_KEY_3 && scenes.size() > 2) {
            activeSceneIndex = 2;
            std::cout << "Prepnuto na scenu: 3 (" << scenes[2]->getName() << ")" << std::endl;
        }
        else if (key == GLFW_KEY_4 && scenes.size() > 3) {
            activeSceneIndex = 3;
            std::cout << "Prepnuto na scenu: 4 (" << scenes[3]->getName() << ")" << std::endl;
        }
        else if (key == GLFW_KEY_5 && scenes.size() > 4) {
            activeSceneIndex = 4;
            std::cout << "Prepnuto na scenu: 5 (" << scenes[4]->getName() << ")" << std::endl;
        }
    }
}