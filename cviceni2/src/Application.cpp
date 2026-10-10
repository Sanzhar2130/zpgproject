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
    glClearColor(0.04f, 0.04f, 0.07f, 1.0f); // Cosmic dark blue-grey background

    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "Renderer:       " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "Controls:\n"
        << "  [1] - Scene 1: Solar System (Planets with vertex colors)\n"
        << "  [2] - Scene 2: Forest Landscape\n"
        << "  [3] - Scene 3: Sphere (Abs Normals Close-up)\n"
        << "  [4] - Scene 4: Student Login (ZHA0067 Large View)\n"
        << "  [ESC] - Exit\n" << std::endl;
}

void Application::createShaders() {
    Shader vertShader(GL_VERTEX_SHADER, "shaders/transform.vert");

    // Shader 0: Shading with absolute normal vectors
    Shader fragAbs(GL_FRAGMENT_SHADER, "shaders/abs_normals.frag");
    shaders.push_back(new ShaderProgram(vertShader, fragAbs));

    // Shader 1: Directional lighting with uniform base color
    Shader fragColor(GL_FRAGMENT_SHADER, "shaders/color_uniform.frag");
    shaders.push_back(new ShaderProgram(vertShader, fragColor));

    // Shader 2: Planet texture/color interpolated from vertex data (sun.h, earth.h, moon.h)
    Shader fragVertexColor(GL_FRAGMENT_SHADER, "shaders/sphere.frag");
    shaders.push_back(new ShaderProgram(vertShader, fragVertexColor));
}

void Application::createModels() {
    // 0: Triangle
    models.push_back(new Model(triangleVertices, sizeof(triangleVertices), 3));

    // 1: Base sphere (2880 vertices)
    models.push_back(new Model(sphere, sizeof(sphere), 2880));

    // 2: Tree
    GLsizei treeCount = sizeof(tree) / (6 * sizeof(float));
    models.push_back(new Model(tree, sizeof(tree), treeCount));

    // 3: Bushes (8730 vertices)
    models.push_back(new Model(bushes, sizeof(bushes), 8730));

    // 4: Plain (6 vertices)
    models.push_back(new Model(plain, sizeof(plain), 6));

    // 5: Gift box (66624 vertices)
    models.push_back(new Model(gift, sizeof(gift), 66624));

    // 6: Student Login Model (ZHA0067)
    GLsizei loginCount = sizeof(zha0067) / (6 * sizeof(float));
    models.push_back(new Model(zha0067, sizeof(zha0067), loginCount));

    // 7: Sun with vertex colors
    GLsizei sunCount = sizeof(sun) / (6 * sizeof(float));
    models.push_back(new Model(sun, sizeof(sun), sunCount));

    // 8: Earth with vertex colors
    GLsizei earthCount = sizeof(earth) / (6 * sizeof(float));
    models.push_back(new Model(earth, sizeof(earth), earthCount));

    // 9: Moon with vertex colors
    GLsizei moonCount = sizeof(moon) / (6 * sizeof(float));
    models.push_back(new Model(moon, sizeof(moon), moonCount));
}

void Application::createScenes() {
    ShaderProgram* spAbs = shaders[0];
    ShaderProgram* spColor = shaders[1];
    ShaderProgram* spVertexColor = shaders[2];

    Model* mTriangle = models[0];
    Model* mSphere = models[1];
    Model* mTree = models[2];
    Model* mBushes = models[3];
    Model* mPlain = models[4];
    Model* mGift = models[5];
    Model* mLogin = models[6];
    Model* mSun = models[7];
    Model* mEarth = models[8];
    Model* mMoon = models[9];

    // =========================================================================
    // Scene 1: Solar System (Planets with baked textures + orbits)
    // =========================================================================
    Scene* sceneSolar = new Scene("Scene 1: Solar System");

    // Sun using sun.h model and vertex color shader
    sunObject = new DrawableObject(mSun, spVertexColor, Transformation());
    sceneSolar->addObject(sunObject);

    // Earth using earth.h model and vertex color shader
    earthObject = new DrawableObject(mEarth, spVertexColor, Transformation());
    sceneSolar->addObject(earthObject);

    // Moon using moon.h model and vertex color shader
    moonObject = new DrawableObject(mMoon, spVertexColor, Transformation());
    sceneSolar->addObject(moonObject);

    // Mars using base sphere with uniform reddish-orange color
    marsObject = new DrawableObject(mSphere, spColor, Transformation(), glm::vec3(0.9f, 0.35f, 0.15f));
    sceneSolar->addObject(marsObject);

    // Author login watermark placed cleanly in view
    Transformation solarSignTrans;
    solarSignTrans.translate(glm::vec3(3.8f, -2.6f, 0.0f));
    solarSignTrans.scale(glm::vec3(0.55f));
    sceneSolar->addObject(new DrawableObject(mLogin, spColor, solarSignTrans, glm::vec3(0.3f, 0.85f, 1.0f)));

    scenes.push_back(sceneSolar);

    // =========================================================================
    // Scene 2: Forest Landscape (Close eye-level overview)
    // =========================================================================
    Scene* sceneForest = new Scene("Scene 2: Forest");

    // Ground plane
    Transformation plainTrans;
    plainTrans.translate(glm::vec3(0.0f, -0.6f, 0.0f));
    plainTrans.scale(glm::vec3(12.0f, 1.0f, 12.0f));
    sceneForest->addObject(new DrawableObject(mPlain, spColor, plainTrans, glm::vec3(0.18f, 0.45f, 0.18f)));

    // Sun high above the forest
    Transformation forestSunTrans;
    forestSunTrans.translate(glm::vec3(4.5f, 5.0f, -5.0f));
    forestSunTrans.scale(glm::vec3(1.3f));
    sceneForest->addObject(new DrawableObject(mSun, spVertexColor, forestSunTrans));

    // Trees arranged into depth layers
    float treeCoords[12][3] = {
        {-4.5f, -0.6f, -3.0f}, {-2.5f, -0.6f, -4.5f}, {-0.8f, -0.6f, -2.5f},
        { 1.5f, -0.6f, -4.0f}, { 3.8f, -0.6f, -3.2f}, { 5.2f, -0.6f, -4.8f},
        {-5.2f, -0.6f, -1.0f}, {-3.2f, -0.6f, -1.8f}, {-1.5f, -0.6f, -5.5f},
        { 0.8f, -0.6f, -1.6f}, { 3.0f, -0.6f, -2.2f}, { 4.6f, -0.6f, -1.0f}
    };
    for (int i = 0; i < 12; ++i) {
        Transformation t;
        t.translate(glm::vec3(treeCoords[i][0], treeCoords[i][1], treeCoords[i][2]));
        t.rotate(i * 0.75f, glm::vec3(0.0f, 1.0f, 0.0f));
        t.scale(glm::vec3(0.55f + (i % 3) * 0.1f));
        sceneForest->addObject(new DrawableObject(mTree, spColor, t, glm::vec3(0.12f, 0.55f, 0.2f)));
    }

    // Bushes along the front
    float bushCoords[12][3] = {
        {-4.0f, -0.55f, 1.0f}, {-2.5f, -0.55f, 0.2f}, {-0.8f, -0.55f, 1.2f},
        { 1.2f, -0.55f, 0.4f}, { 2.8f, -0.55f, 0.9f}, { 4.2f, -0.55f, 0.1f},
        {-4.8f, -0.55f, 1.8f}, {-3.3f, -0.55f, 2.0f}, {-1.8f, -0.55f, 2.4f},
        { 0.3f, -0.55f, 1.8f}, { 2.0f, -0.55f, 2.2f}, { 3.6f, -0.55f, 1.6f}
    };
    for (int i = 0; i < 12; ++i) {
        Transformation t;
        t.translate(glm::vec3(bushCoords[i][0], bushCoords[i][1], bushCoords[i][2]));
        t.scale(glm::vec3(1.0f + (i % 4) * 0.18f));
        sceneForest->addObject(new DrawableObject(mBushes, spColor, t, glm::vec3(0.22f, 0.72f, 0.28f)));
    }

    // Gift in center clearing
    Transformation giftTrans;
    giftTrans.translate(glm::vec3(0.0f, -0.1f, 0.2f));
    giftTrans.scale(glm::vec3(0.7f));
    sceneForest->addObject(new DrawableObject(mGift, spColor, giftTrans, glm::vec3(0.9f, 0.2f, 0.2f)));

    // Watermark
    Transformation forestSignTrans;
    forestSignTrans.translate(glm::vec3(3.2f, -1.8f, 1.0f));
    forestSignTrans.scale(glm::vec3(0.5f));
    sceneForest->addObject(new DrawableObject(mLogin, spColor, forestSignTrans, glm::vec3(0.3f, 0.85f, 1.0f)));

    scenes.push_back(sceneForest);

    // =========================================================================
    // Scene 3: Sphere Close-up (Normals Verification + Clear Visible Login)
    // =========================================================================
    Scene* sceneSphere = new Scene("Scene 3: Sphere");

    // Large centered sphere filling most of the screen
    Transformation sphereTrans;
    sphereTrans.scale(glm::vec3(1.35f));
    sceneSphere->addObject(new DrawableObject(mSphere, spAbs, sphereTrans));

    // Clear, close-up student login signature in bottom-right corner
    Transformation sphereSignTrans;
    sphereSignTrans.translate(glm::vec3(1.35f, -1.05f, 0.0f));
    sphereSignTrans.scale(glm::vec3(0.38f));
    sceneSphere->addObject(new DrawableObject(mLogin, spColor, sphereSignTrans, glm::vec3(0.2f, 0.9f, 1.0f)));

    scenes.push_back(sceneSphere);

    // =========================================================================
    // Scene 4: Student Login Model (ZHA0067 Large Center View)
    // =========================================================================
    Scene* sceneLogin = new Scene("Scene 4: Student Login");

    // Login model rendered large and centered right before the camera
    Transformation loginTrans;
    loginTrans.translate(glm::vec3(-0.1f, 0.0f, 0.0f));
    loginTrans.scale(glm::vec3(1.35f));
    sceneLogin->addObject(new DrawableObject(mLogin, spAbs, loginTrans));

    scenes.push_back(sceneLogin);
}

void Application::updateSolarSystem(float currentTime) {
    if (!sunObject || !earthObject || !moonObject || !marsObject) return;

    // 1. Sun: rotates around vertical axis
    glm::mat4 sunMatrix = glm::mat4(1.0f);
    sunMatrix = glm::rotate(sunMatrix, currentTime * 0.25f, glm::vec3(0.0f, 1.0f, 0.0f));
    sunMatrix = glm::scale(sunMatrix, glm::vec3(1.5f));
    sunObject->setTransformation(Transformation(sunMatrix));

    // 2. Earth: Orbit around Sun + axis rotation + scale
    float earthOrbitRadius = 5.2f;
    glm::mat4 earthOrbit = glm::rotate(glm::mat4(1.0f), currentTime * 0.75f, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 earthCenterMatrix = glm::translate(earthOrbit, glm::vec3(earthOrbitRadius, 0.0f, 0.0f));

    glm::mat4 earthMatrix = glm::rotate(earthCenterMatrix, currentTime * 2.8f, glm::vec3(0.0f, 1.0f, 0.0f));
    earthMatrix = glm::scale(earthMatrix, glm::vec3(0.72f));
    earthObject->setTransformation(Transformation(earthMatrix));

    // 3. Moon: attached hierarchically to Earth + orbit around Earth
    float moonOrbitRadius = 1.35f;
    glm::mat4 moonMatrix = glm::rotate(earthCenterMatrix, currentTime * 3.8f, glm::vec3(0.0f, 1.0f, 0.2f));
    moonMatrix = glm::translate(moonMatrix, glm::vec3(moonOrbitRadius, 0.0f, 0.0f));
    moonMatrix = glm::scale(moonMatrix, glm::vec3(0.28f));
    moonObject->setTransformation(Transformation(moonMatrix));

    // 4. Mars: outer independent orbit
    float marsOrbitRadius = 8.2f;
    glm::mat4 marsMatrix = glm::rotate(glm::mat4(1.0f), currentTime * 0.42f, glm::vec3(0.0f, 1.0f, 0.0f));
    marsMatrix = glm::translate(marsMatrix, glm::vec3(marsOrbitRadius, 0.0f, 0.0f));
    marsMatrix = glm::rotate(marsMatrix, currentTime * 2.2f, glm::vec3(0.0f, 1.0f, 0.0f));
    marsMatrix = glm::scale(marsMatrix, glm::vec3(0.52f));
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

        // Aspect ratio projection
        glm::mat4 projection = glm::perspective(
            glm::radians(45.0f),
            static_cast<float>(windowWidth) / static_cast<float>(windowHeight),
            0.1f,
            100.0f
        );

        // Dedicated close-up camera per scene
        glm::mat4 view = glm::mat4(1.0f);
        if (activeSceneIndex == 0) {
            // Solar System: close-up elevated view showing all planetary details
            view = glm::lookAt(
                glm::vec3(0.0f, 5.5f, 12.5f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 1.0f, 0.0f)
            );
        } else if (activeSceneIndex == 1) {
            // Forest: ground eye-level standing in front of trees
            view = glm::lookAt(
                glm::vec3(0.0f, 1.2f, 7.5f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 1.0f, 0.0f)
            );
        } else if (activeSceneIndex == 2) {
            // Sphere: front-facing close-up occupying the window
            view = glm::lookAt(
                glm::vec3(0.0f, 0.0f, 3.8f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 1.0f, 0.0f)
            );
        } else if (activeSceneIndex == 3) {
            // Student Login: direct readable text view right in front of camera
            view = glm::lookAt(
                glm::vec3(0.0f, 0.0f, 3.2f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 1.0f, 0.0f)
            );
        }

        // Render current scene
        if (activeSceneIndex < scenes.size() && scenes[activeSceneIndex] != nullptr) {
            scenes[activeSceneIndex]->setMatrices(view, projection);
            scenes[activeSceneIndex]->render();
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}