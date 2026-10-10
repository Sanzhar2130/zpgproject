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

    if (name.find("Solar") != std::string::npos || name.find("soustava") != std::string::npos) {
        wmComp->add(std::make_shared<Rotation>(glm::radians(-25.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
        wmComp->add(std::make_shared<Translation>(11.5f, -7.5f, 3.5f));
        wmComp->add(std::make_shared<Scale>(1.55f, 1.55f, 0.001f));
    }
    else if (name.find("Les") != std::string::npos || name.find("Forest") != std::string::npos) {
        // =====================================================================
        // Сцена 3: Лес (камера на (0, 2, 6) с наклоном вниз)
        // =====================================================================
        // 1. Поворот навстречу лучу зрения камеры, чтобы текст был плоским и четким
        wmComp->add(std::make_shared<Rotation>(glm::radians(-18.5f), glm::vec3(1.0f, 0.0f, 0.0f)));
        // 2. Смещение в правый нижний угол экрана перед платформой
        wmComp->add(std::make_shared<Translation>(2.2f, -0.15f, 2.6f));
        // 3. Увеличенный масштаб под дистанцию 6 единиц
        wmComp->add(std::make_shared<Scale>(0.42f, 0.42f, 0.001f));
    }
    else {
        // =====================================================================
        // Сцены 1 и 2: Треугольник и Сфера (камера на Z = 2.5)
        // =====================================================================
        wmComp->add(std::make_shared<Translation>(1.05f, -0.75f, 0.0f));
        wmComp->add(std::make_shared<Scale>(0.18f, 0.18f, 0.001f));
    }

    // Однородная заливка бирюзовым цветом шейдером shaders[2] убирает артефакты нормалей[cite: 44]
    scene->addObject(new DrawableObject(models[6], shaders[2], wmComp, glm::vec3(0.0f, 0.8f, 1.0f)));
}

void Application::createScenes() {
    // =========================================================================
    // СЦЕНА 1: Базовый треугольник (Triangle)
    // =========================================================================
    Scene* scene1 = new Scene("1: Trojuhelnik");
    auto triComp = std::make_shared<CompositeTransformation>();
    triComp->add(std::make_shared<Scale>(1.0f));
    scene1->addObject(new DrawableObject(models[0], shaders[0], triComp));
    addWatermark(scene1);
    scenes.push_back(scene1);

    // =========================================================================
    // СЦЕНА 2: Сфера с градиентом нормалей (Sphere)
    // =========================================================================
    Scene* scene2 = new Scene("2: Koule");
    auto sphereComp = std::make_shared<CompositeTransformation>();
    sphereComp->add(std::make_shared<Scale>(0.85f));
    scene2->addObject(new DrawableObject(models[1], shaders[1], sphereComp));
    addWatermark(scene2);
    scenes.push_back(scene2);

    // =========================================================================
    // СЦЕНА 3: Наполненный 3D-лес (Forest)
    // =========================================================================
    // =========================================================================
    // СЦЕНА 3: Наполненный 3D-лес на платформе
    // =========================================================================
    Scene* scene3 = new Scene("3: Les");

    // 1. Зеленая платформа земли (plain)[cite: 44]
    auto plainComp = std::make_shared<CompositeTransformation>();
    plainComp->add(std::make_shared<Translation>(0.0f, -0.65f, -0.9f));
    plainComp->add(std::make_shared<Scale>(4.8f, 1.0f, 4.2f));
    scene3->addObject(new DrawableObject(models[2], shaders[2], plainComp, glm::vec3(0.22f, 0.52f, 0.22f)));

    // 2. Солнце высоко в небе справа (полностью над деревьями)[cite: 44]
    auto forestSunComp = std::make_shared<CompositeTransformation>();
    forestSunComp->add(std::make_shared<Translation>(2.35f, 1.95f, -1.0f)); // поднято выше крон
    forestSunComp->add(std::make_shared<Scale>(0.58f));                     // крупный видимый диск
    scene3->addObject(new DrawableObject(models[1], shaders[2], forestSunComp, glm::vec3(1.0f, 0.95f, 0.15f)));

    // 3. 14 деревьев по ширине поляны[cite: 44]
    float treeX[] = { -1.85f, -1.60f, -1.35f, -1.10f, -0.85f, -0.60f, -0.35f,
                      -0.10f,  0.15f,  0.40f,  0.65f,  0.90f,  1.10f,  1.25f };
    for (int i = 0; i < 14; i++) {
        auto treeComp = std::make_shared<CompositeTransformation>();
        float z = (i % 2 == 0) ? -1.25f : -0.95f;
        float y = (i % 2 == 0) ? -0.55f : -0.58f;
        float s = (i % 2 == 0) ? 0.34f : 0.37f;

        treeComp->add(std::make_shared<Translation>(treeX[i], y, z));
        treeComp->add(std::make_shared<Rotation>(glm::radians(i * 25.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
        treeComp->add(std::make_shared<Scale>(s));

        glm::vec3 treeColor = (i % 2 == 0) ? glm::vec3(0.14f, 0.52f, 0.15f) : glm::vec3(0.11f, 0.46f, 0.13f);
        scene3->addObject(new DrawableObject(models[3], shaders[2], treeComp, treeColor));
    }

    // 4. Кусты перед деревьями[cite: 44]
    for (int i = 0; i < 9; i++) {
        auto bushComp = std::make_shared<CompositeTransformation>();
        float bx = -1.45f + i * 0.35f;
        bushComp->add(std::make_shared<Translation>(bx, -0.53f, 0.05f));
        bushComp->add(std::make_shared<Scale>(0.19f));
        scene3->addObject(new DrawableObject(models[4], shaders[2], bushComp, glm::vec3(0.18f, 0.65f, 0.22f)));
    }

    addWatermark(scene3);
    scenes.push_back(scene3);

    // =========================================================================
    // СЦЕНА 4: Персональная 3D-модель логина (ZHA0067)
    // =========================================================================
    Scene* scene4 = new Scene("4: Model ZHA0067");
    auto loginComp = std::make_shared<CompositeTransformation>();
    
    // БЕЗ поворотов: строго фронтальный вид, только масштабирование
    loginComp->add(std::make_shared<Scale>(0.85f));
    
    scene4->addObject(new DrawableObject(models[6], shaders[1], loginComp));
    scenes.push_back(scene4);

    // =========================================================================
    // СЦЕНА 5: Солнечная система с иерархией орбит
    // =========================================================================
    Scene* scene5 = new Scene("5: Slunecni soustava");

    // 1. СОЛНЦЕ (Центр мира, собственное медленное вращение)
    auto sunComp = std::make_shared<CompositeTransformation>();
    sunSelfRotation = std::make_shared<Rotation>(0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    sunComp->add(sunSelfRotation);
    sunComp->add(std::make_shared<Scale>(1.6f));
    scene5->addObject(new DrawableObject(models[7], shaders[0], sunComp));

    // 2. ЗЕМЛЯ (Вращение по орбите вокруг Солнца + смещение + собственное вращение)
    auto earthComp = std::make_shared<CompositeTransformation>();
    earthOrbitRotation = std::make_shared<Rotation>(0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    earthSelfRotation = std::make_shared<Rotation>(0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    earthComp->add(earthOrbitRotation);
    earthComp->add(std::make_shared<Translation>(7.0f, 0.0f, 0.0f)); // радиус орбиты Земли
    earthComp->add(earthSelfRotation);
    earthComp->add(std::make_shared<Scale>(0.6f));
    scene5->addObject(new DrawableObject(models[8], shaders[0], earthComp));

    // 3. ЛУНА (Наследует орбиту Земли + собственная орбита вокруг Земли + масштаб)
    auto moonComp = std::make_shared<CompositeTransformation>();
    moonOrbitRotation = std::make_shared<Rotation>(0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    moonSelfRotation = std::make_shared<Rotation>(0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    // Переход в систему координат Земли:
    moonComp->add(earthOrbitRotation);
    moonComp->add(std::make_shared<Translation>(7.0f, 0.0f, 0.0f));
    // Собственная орбита вокруг центра Земли:
    moonComp->add(moonOrbitRotation);
    moonComp->add(std::make_shared<Translation>(1.5f, 0.0f, 0.0f)); // расстояние от Земли до Луны
    moonComp->add(moonSelfRotation);
    moonComp->add(std::make_shared<Scale>(0.22f));
    scene5->addObject(new DrawableObject(models[9], shaders[0], moonComp));

    addWatermark(scene5);
    scenes.push_back(scene5);
}

void Application::run() {
    glEnable(GL_DEPTH_TEST);

    std::cout << "\n=======================================================\n";
    std::cout << "Aplikace spustena! Prepinani scen klavesami 1 - 5:\n";
    std::cout << " 1: Trojuhelnik (RGB interpolace)\n";
    std::cout << " 2: Koule (Normal shading)\n";
    std::cout << " 3: Les (Krajina se sluncem vpravo nahore)\n";
    std::cout << " 4: Model studenta (ZHA0067)\n";
    std::cout << " 5: Slunecni soustava (Slunce, Zeme, Mesic)\n";
    std::cout << "=======================================================\n";

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float time = static_cast<float>(glfwGetTime());
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
        }
        else if (activeSceneIndex == 2) {
            // Сцена 3: Лес (камера отдалена назад на 35 и поднята на 15, чтобы охватить весь ландшафт)
            view = glm::lookAt(
                glm::vec3(0.0f, 1.0f, 8.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 1.0f, 0.0f)
            );
        }
        else if (activeSceneIndex == 3) {
            // Сцена 4: Модель студента ZHA0067
            view = glm::lookAt(
                glm::vec3(0.0f, 0.0f, 2.5f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 1.0f, 0.0f)
            );
        }
        else {
            // Сцены 1 и 2: Треугольник и Сфера
            view = glm::lookAt(
                glm::vec3(0.0f, 0.0f, 2.5f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 1.0f, 0.0f)
            );
        }

        // 4. Отправка матриц во все шейдерные программы
        for (ShaderProgram* sp : shaders) {
            sp->use(); //[cite: 22]
            sp->setUniform("viewMatrix", view);
            sp->setUniform("projectMatrix", projection);
        }

        // 5. Отрисовка активной сцены
        if (activeSceneIndex < scenes.size() && scenes[activeSceneIndex] != nullptr) {
            scenes[activeSceneIndex]->render(); //[cite: 22]
        }

        glfwSwapBuffers(window); //[cite: 22]
        glfwPollEvents(); //[cite: 22]
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
                std::cout << "Prepnuto na scenu: " << scenes[idx]->getName() << std::endl;
            }
        }
    }
}