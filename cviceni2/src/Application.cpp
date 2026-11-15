/**
 * @file Application.cpp
 * @author Student (login: ZHA0067)
 * @brief Application lifecycle coordinator with static scenes and scene switcher.
 */

#include "Application.h"
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
#include <ctime>
#include <algorithm>

#include "../models/sun.h"
#include "../models/earth.h"
#include "../models/moon.h"
#include "../models/sphere.h"
#include "../models/plain.h"
#include "../models/tree.h"
#include "../models/bushes.h"
#include "../models/gift.h"
#include "../models/zha0067.h"
#include "../models/clock.h"
#include "../models/rocket.h"
#include "../models/Shrek.h"
#include "../models/Fiona.h"
#include "../models/Skybox.h"
#include "../models/EB408.h"
#include "../models/door.h"
#include "../models/door_frame.h"
#include "../models/whiteboard.h"
#include <../models/table.h>
#include <../models/orange_chair.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

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
        if (app->camera) {
            float aspect = (height > 0) ? (static_cast<float>(width) / static_cast<float>(height)) : 1.0f;
            app->camera->setAspect(aspect);
        }
    }
}

void Application::cursor_pos_callback(GLFWwindow* window, double xpos, double ypos) {
    Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
    if (app) {
        app->handleMouse(xpos, ypos);
    }
}

Application::Application()
    : window(nullptr), windowWidth(1024), windowHeight(768), activeSceneIndex(0),
    camera(nullptr), lastX(512.0), lastY(384.0), firstMouse(true),
    isMousePressed(false), deltaTime(0.0f), lastFrame(0.0f),
    clockShader(nullptr), treeShader(nullptr), skyboxShader(nullptr),
    phongShader(nullptr), blinnShader(nullptr), ambientShader(nullptr),
    blinnColorShader(nullptr), vertPhongBad(nullptr), fragPhongBad(nullptr),
    nightShader(nullptr), textureShader(nullptr) {}

void Application::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
    if (app) {
        app->handleMouseButton(button, action, mods);
    }
}

void Application::handleMouseButton(int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_RIGHT || button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            isMousePressed = true;
            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);
            lastX = xpos;
            lastY = ypos;

            if (button == GLFW_MOUSE_BUTTON_LEFT) {
                bool isCtrlPressed = ((mods & GLFW_MOD_CONTROL) != 0) ||
                    (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) ||
                    (glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS);

                forceStaticPlaceOnce = isCtrlPressed;
                pendingClickAction = true;
                pickCursorX = xpos;
                pickCursorY = ypos;
            }
        }
        else if (action == GLFW_RELEASE) {
            isMousePressed = false;
        }
    }
}
void Application::performPicking() {
    if (!pendingClickAction) return;
    pendingClickAction = false;

    InteractionMode activeMode = forceStaticPlaceOnce ? InteractionMode::PlaceStatic : interactionMode;
    forceStaticPlaceOnce = false;

    int winW = 0, winH = 0;
    int fbW = 0, fbH = 0;
    glfwGetWindowSize(window, &winW, &winH);
    glfwGetFramebufferSize(window, &fbW, &fbH);

    if (winW <= 0 || winH <= 0 || fbW <= 0 || fbH <= 0) return;

    double scaleX = static_cast<double>(fbW) / static_cast<double>(winW);
    double scaleY = static_cast<double>(fbH) / static_cast<double>(winH);

    GLint x = static_cast<GLint>(pickCursorX * scaleX);
    GLint y = static_cast<GLint>(pickCursorY * scaleY);
    GLint newy = fbH - y;

    if (x < 0 || x >= fbW || newy < 0 || newy >= fbH) return;

    GLubyte color[4] = { 0, 0, 0, 0 };
    GLfloat depth = 0.0f;
    GLuint index = 0;

    glReadPixels(x, newy, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, color);
    glReadPixels(x, newy, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
    glReadPixels(x, newy, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_INT, &index);

    if (activeSceneIndex >= scenes.size() || scenes[activeSceneIndex] == nullptr) return;
    Scene* currentScene = scenes[activeSceneIndex];

    bool isSupportedScene = (activeSceneIndex == 2 || activeSceneIndex == 7 || activeSceneIndex == 8);

    if (activeMode == InteractionMode::PlaceStatic && camera != nullptr) {
        if (!isSupportedScene) {
            std::cout << "Object placement is enabled in scenes: 3 (Forest), 8 (Shrek), 9 (EB408)." << std::endl;
            return;
        }
        if (depth >= 1.0f) {
            std::cout << "Click on a surface or object, not on the skybox." << std::endl;
            return;
        }

        glm::vec3 screenPos(static_cast<float>(x), static_cast<float>(newy), depth);
        glm::mat4 view = camera->getViewMatrix();
        glm::mat4 projection = camera->getProjectionMatrix();
        glm::vec4 viewport(0.0f, 0.0f, static_cast<float>(fbW), static_cast<float>(fbH));
        glm::vec3 worldPos = glm::unProject(screenPos, view, projection, viewport);

        auto getRandomFloat = [](float minVal, float maxVal) -> float {
            return minVal + static_cast<float>(std::rand()) / (static_cast<float>(RAND_MAX / (maxVal - minVal)));
            };

        float rotAngle = getRandomFloat(0.0f, 360.0f);
        auto objComp = std::make_shared<CompositeTransformation>();
        DrawableObject* newObj = nullptr;

        if (activeSceneIndex == 2 || activeSceneIndex == 7) {
            if (placeAlternateToggle) {
                float treeScale = getRandomFloat(0.28f, 0.45f);
                objComp->add(std::make_shared<Translation>(worldPos));
                objComp->add(std::make_shared<Rotation>(glm::radians(rotAngle), glm::vec3(0.0f, 1.0f, 0.0f)));
                objComp->add(std::make_shared<Scale>(treeScale));

                glm::vec3 treeColor(getRandomFloat(0.08f, 0.22f), getRandomFloat(0.45f, 0.68f), getRandomFloat(0.10f, 0.20f));
                newObj = new DrawableObject(models[3], nightShader, objComp, treeColor);
            }
            else {
                float bushScale = getRandomFloat(0.16f, 0.28f);
                objComp->add(std::make_shared<Translation>(worldPos.x, worldPos.y + 0.07f, worldPos.z));
                objComp->add(std::make_shared<Rotation>(glm::radians(rotAngle), glm::vec3(0.0f, 1.0f, 0.0f)));
                objComp->add(std::make_shared<Scale>(bushScale));

                glm::vec3 bushColor(0.15f, getRandomFloat(0.55f, 0.78f), 0.18f);
                newObj = new DrawableObject(models[4], nightShader, objComp, bushColor);
            }
        }
        else if (activeSceneIndex == 8) {
            if (placeAlternateToggle) {
                objComp->add(std::make_shared<Translation>(worldPos));
                objComp->add(std::make_shared<Rotation>(glm::radians(rotAngle), glm::vec3(0.0f, 1.0f, 0.0f)));
                objComp->add(std::make_shared<Rotation>(glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
                objComp->add(std::make_shared<Scale>(0.3f));
                newObj = new DrawableObject(models[21], textureShader, objComp, glm::vec3(1.0f), ocTex);
            }
            else {
                objComp->add(std::make_shared<Translation>(worldPos));
                objComp->add(std::make_shared<Rotation>(glm::radians(rotAngle), glm::vec3(0.0f, 1.0f, 0.0f)));
                objComp->add(std::make_shared<Rotation>(glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
                objComp->add(std::make_shared<Scale>(0.3f));
                newObj = new DrawableObject(models[20], textureShader, objComp, glm::vec3(1.0f), tableTex);
            }
        }

        if (newObj != nullptr) {
            currentScene->addObject(newObj);
            placeAlternateToggle = !placeAlternateToggle;
            std::printf("Placed static object (ID: %u) at [%.3f, %.3f, %.3f]\n",
                newObj->getID(), worldPos.x, worldPos.y, worldPos.z);
        }
        return;
    }

    if (activeMode == InteractionMode::PlaceMoving && camera != nullptr) {
        if (!isSupportedScene) {
            std::cout << "Moving object creation is enabled in scenes: 3 (Forest), 8 (Shrek), 9 (EB408)." << std::endl;
            return;
        }
        if (depth >= 1.0f) {
            std::cout << "Click on a surface or object, not on the skybox." << std::endl;
            return;
        }

        glm::vec3 screenPos(static_cast<float>(x), static_cast<float>(newy), depth);
        glm::mat4 view = camera->getViewMatrix();
        glm::mat4 projection = camera->getProjectionMatrix();
        glm::vec4 viewport(0.0f, 0.0f, static_cast<float>(fbW), static_cast<float>(fbH));
        glm::vec3 worldPos = glm::unProject(screenPos, view, projection, viewport);

        if (!hasFirstLinePoint) {
            linePointA = worldPos;
            hasFirstLinePoint = true;
            std::printf("Point A set at [%.3f, %.3f, %.3f]. Click second point B...\n",
                linePointA.x, linePointA.y, linePointA.z);
        }
        else {
            glm::vec3 linePointB = worldPos;
            hasFirstLinePoint = false;

            auto moveTrans = std::make_shared<Translation>(linePointA);
            auto objComp = std::make_shared<CompositeTransformation>();
            objComp->add(moveTrans);

            DrawableObject* movingObj = nullptr;
            float moveSpeed = 1.5f;

            if (activeSceneIndex == 2) {
                objComp->add(std::make_shared<Scale>(0.42f));
                movingObj = new DrawableObject(models[5], shaders[2], objComp, glm::vec3(1.0f, 0.35f, 0.15f));
                moveSpeed = 1.8f;
            }
            else if (activeSceneIndex == 8) {
                objComp->add(std::make_shared<Rotation>(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
                objComp->add(std::make_shared<Scale>(0.25f));
                movingObj = new DrawableObject(models[13], textureShader, objComp, glm::vec3(1.0f), shrekTex);
                moveSpeed = 0.65f;
            }
            else {
                objComp->add(std::make_shared<Scale>(0.42f));
                movingObj = new DrawableObject(models[5], shaders[2], objComp, glm::vec3(0.2f, 0.85f, 1.0f));
                moveSpeed = 1.8f;
            }

            currentScene->addObject(movingObj);

            MovingObject mInfo;
            mInfo.sceneIndex = activeSceneIndex;
            mInfo.objectID = movingObj->getID();
            mInfo.translation = moveTrans;
            mInfo.pointA = linePointA;
            mInfo.pointB = linePointB;
            mInfo.t = 0.0f;
            mInfo.speed = moveSpeed;
            mInfo.direction = 1;
            movingObjects.push_back(mInfo);

            std::printf("Spawned moving model (ID: %u) between A[%.2f, %.2f, %.2f] and B[%.2f, %.2f, %.2f]\n",
                movingObj->getID(),
                linePointA.x, linePointA.y, linePointA.z,
                linePointB.x, linePointB.y, linePointB.z);
        }
        return;
    }

    std::printf("Clicked on pixel %d, %d, color %02hhx%02hhx%02hhx%02hhx, depth %f, stencil index %u\n",
        x, newy, color[0], color[1], color[2], color[3], depth, index);

    if (index != 0) {
        for (auto it = movingObjects.begin(); it != movingObjects.end(); ++it) {
            if (it->sceneIndex == activeSceneIndex && it->objectID == index) {
                currentScene->removeObjectByID(index);
                movingObjects.erase(it);
                std::cout << "Interaction: Moving model (ID: " << index << ") clicked and deleted!" << std::endl;
                return;
            }
        }
    }

    currentScene->selectByID(index);
    DrawableObject* selected = currentScene->getSelectedObject();
    if (selected != nullptr) {
        std::cout << "Selected object ID: " << selected->getID()
            << " in scene [" << currentScene->getName() << "]" << std::endl;
    }
    else {
        std::cout << "No object selected (background clicked)." << std::endl;
    }
}

void Application::handleMouse(double xpos, double ypos) {
    if (!isMousePressed) {
        lastX = xpos;
        lastY = ypos;
        return;
    }

    float xoffset = static_cast<float>(xpos - lastX);
    float yoffset = static_cast<float>(lastY - ypos);

    lastX = xpos;
    lastY = ypos;

    if (camera) {
        camera->handleMouseMovement(xoffset, yoffset);
    }
}

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

void Application::createShaders() {
    for (ShaderProgram* sp : shaders) {
        delete sp;
    }
    shaders.clear();

    Shader vertColor(GL_VERTEX_SHADER, "shaders/vertex_color.vert");
    Shader fragColor(GL_FRAGMENT_SHADER, "shaders/vertex_color.frag");
    ShaderProgram* spColor = new ShaderProgram(vertColor, fragColor);
    shaders.push_back(spColor);

    Shader vertNormal(GL_VERTEX_SHADER, "shaders/transform.vert");
    Shader fragAbsNormal(GL_FRAGMENT_SHADER, "shaders/abs_normals.frag");
    ShaderProgram* spNormal = new ShaderProgram(vertNormal, fragAbsNormal);
    shaders.push_back(spNormal);

    Shader fragUniformColor(GL_FRAGMENT_SHADER, "shaders/color_uniform.frag");
    ShaderProgram* spUniform = new ShaderProgram(vertNormal, fragUniformColor);
    shaders.push_back(spUniform);

    Shader vertClock(GL_VERTEX_SHADER, "shaders/clock.vert");
    Shader fragClock(GL_FRAGMENT_SHADER, "shaders/clock.frag");
    clockShader = new ShaderProgram(vertClock, fragClock);
    shaders.push_back(clockShader);

    Shader vertTree(GL_VERTEX_SHADER, "shaders/tree.vert");
    Shader fragTree(GL_FRAGMENT_SHADER, "shaders/tree.frag");
    treeShader = new ShaderProgram(vertTree, fragTree);
    shaders.push_back(treeShader);

    Shader vertAmbient(GL_VERTEX_SHADER, "shaders/ambient.vert");
    Shader fragAmbient(GL_FRAGMENT_SHADER, "shaders/ambient.frag");
    ambientShader = new ShaderProgram(vertAmbient, fragAmbient);
    shaders.push_back(ambientShader);

    Shader vertLambert(GL_VERTEX_SHADER, "shaders/lambert.vert");
    Shader fragLambert(GL_FRAGMENT_SHADER, "shaders/lambert.frag");
    ShaderProgram* spLambert = new ShaderProgram(vertLambert, fragLambert);
    shaders.push_back(spLambert);

    Shader vertPhong(GL_VERTEX_SHADER, "shaders/phong.vert");
    Shader fragPhong(GL_FRAGMENT_SHADER, "shaders/phong.frag");
    phongShader = new ShaderProgram(vertPhong, fragPhong);
    shaders.push_back(phongShader);

    Shader vertBlinn(GL_VERTEX_SHADER, "shaders/blinn.vert");
    Shader fragBlinn(GL_FRAGMENT_SHADER, "shaders/blinn.frag");
    blinnShader = new ShaderProgram(vertBlinn, fragBlinn);
    shaders.push_back(blinnShader);

    Shader vertBlinnColor(GL_VERTEX_SHADER, "shaders/blinn_color.vert");
    Shader fragBlinnColor(GL_FRAGMENT_SHADER, "shaders/blinn_color.frag");
    blinnColorShader = new ShaderProgram(vertBlinnColor, fragBlinnColor);
    shaders.push_back(blinnColorShader);

    Shader vertNight(GL_VERTEX_SHADER, "shaders/night_forest.vert");
    Shader fragNight(GL_FRAGMENT_SHADER, "shaders/night_forest.frag");
    nightShader = new ShaderProgram(vertNight, fragNight);
    shaders.push_back(nightShader);

    Shader vertTex(GL_VERTEX_SHADER, "shaders/texture.vert");
    Shader fragTex(GL_FRAGMENT_SHADER, "shaders/texture.frag");
    textureShader = new ShaderProgram(vertTex, fragTex);
    shaders.push_back(textureShader);

    Shader vertSkybox(GL_VERTEX_SHADER, "shaders/skybox.vert");
    Shader fragSkybox(GL_FRAGMENT_SHADER, "shaders/skybox.frag");
    skyboxShader = new ShaderProgram(vertSkybox, fragSkybox);
    shaders.push_back(skyboxShader);

    for (ShaderProgram* sp : shaders) {
        camera->attach(sp);
    }
}

GLuint loadTexture(const char* filePath) {
    int text_width, text_height, channels;
    stbi_set_flip_vertically_on_load(true);

    unsigned char* data = stbi_load(filePath, &text_width, &text_height, &channels, 4);
    if (!data) {
        std::cerr << "Error loading texture: " << filePath << std::endl;
        return 0;
    }

    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, text_width, text_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);

    return tex;
}

GLuint loadCubemap(const std::vector<const char*>& faces) {
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    stbi_set_flip_vertically_on_load(false);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++) {
        unsigned char* data = stbi_load(faces[i], &width, &height, &nrChannels, 0);
        if (data) {
            GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else {
            std::cerr << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}

void Application::createModels() {
    models.push_back(new Model(triangleVertices, sizeof(triangleVertices), 3));
    models.push_back(new Model(sphere, sizeof(sphere), 2880));
    models.push_back(new Model(plain, sizeof(plain), 6, false, true));
    models.push_back(new Model(tree, sizeof(tree), 92814));
    models.push_back(new Model(bushes, sizeof(bushes), 8730));
    models.push_back(new Model(gift, sizeof(gift), 66624));
    models.push_back(new Model(zha0067, sizeof(zha0067), 1776));

    models.push_back(new Model(sun, sizeof(sun), 11520, true));
    models.push_back(new Model(earth, sizeof(earth), 11520, true));
    models.push_back(new Model(moon, sizeof(moon), 11520, true));

    models.push_back(new Model(clockModel, sizeof(clockModel), static_cast<GLsizei>(vertexCount), true));
    models.push_back(new Model(rocketModel, sizeof(rocketModel), static_cast<GLsizei>(vertexRocketCount), true));

    models.push_back(new Model(plain, sizeof(plain), 6, false, true));

    GLsizei shrekVerticesCount = static_cast<GLsizei>(sizeof(ShrekModel) / (8 * sizeof(float)));
    models.push_back(new Model(ShrekModel, sizeof(ShrekModel), shrekVerticesCount, false, true));

    GLsizei fionaVerticesCount = static_cast<GLsizei>(sizeof(FionaModel) / (8 * sizeof(float)));
    models.push_back(new Model(FionaModel, sizeof(FionaModel), fionaVerticesCount, false, true));

    models.push_back(new Model(CubeModel, sizeof(CubeModel), CubeVertexCount, false, true));

    models.push_back(new Model(SM_EB408Model, sizeof(SM_EB408Model), SM_EB408VertexCount, false, true));
    models.push_back(new Model(SM_DoorFrame_vertices, sizeof(SM_DoorFrame_vertices), SM_DoorFrame_vertex_count, false, true));
    models.push_back(new Model(SM_Door_vertices, sizeof(SM_Door_vertices), SM_Door_vertex_count, false, true));
    models.push_back(new Model(SM_Whiteboard_vertices, sizeof(SM_Whiteboard_vertices), SM_Whiteboard_vertex_count, false, true));
    models.push_back(new Model(SM_Table_vertices, sizeof(SM_Table_vertices), SM_Table_vertex_count, false, true));
    models.push_back(new Model(OrangeChair_vertices, sizeof(OrangeChair_vertices), OrangeChair_vertex_count, false, true));
}

void Application::addWatermark(Scene* scene) {
    auto wmComp = std::make_shared<CompositeTransformation>();
    std::string name = scene->getName();

    if (name.find("Solar") != std::string::npos || name.find("SolarSystem") != std::string::npos) {
        wmComp->add(std::make_shared<Rotation>(glm::radians(-25.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
        wmComp->add(std::make_shared<Translation>(11.5f, -7.5f, 3.5f));
        wmComp->add(std::make_shared<Scale>(1.55f, 1.55f, 0.001f));
    }
    else if (name.find("Forest") != std::string::npos || name.find("Woodland") != std::string::npos) {
        wmComp->add(std::make_shared<Rotation>(glm::radians(-18.5f), glm::vec3(1.0f, 0.0f, 0.0f)));
        wmComp->add(std::make_shared<Translation>(-4.0f, 3.5f, 0.0f));
        wmComp->add(std::make_shared<Scale>(0.6f, 0.6f, 0.001f));
    }
    else if (name.find("Lighting") != std::string::npos || name.find("Models") != std::string::npos) {
        wmComp->add(std::make_shared<Translation>(2.0f, -1.5f, 0.0f));
        wmComp->add(std::make_shared<Scale>(0.6f, 0.6f, 0.001f));
    }
    else if (name.find("EB408 Model") != std::string::npos) {
        wmComp->add(std::make_shared<Translation>(-1.1f, 0.8f, 1.5f));
        wmComp->add(std::make_shared<Rotation>(glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
        wmComp->add(std::make_shared<Scale>(0.15f, 0.15f, 0.001f));
    }
    else {
        wmComp->add(std::make_shared<Translation>(1.5f, -0.75f, 0.0f));
        wmComp->add(std::make_shared<Scale>(0.1f, 0.1f, 0.001f));
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
    glm::vec3 fireflyPos(0.0f, 0.8f, 0.0f);
    glm::vec3 fireflyColor(0.9f, 1.0f, 0.3f);

    auto plainComp = std::make_shared<CompositeTransformation>();
    plainComp->add(std::make_shared<Translation>(0.0f, -0.65f, 0.0f));
    plainComp->add(std::make_shared<Scale>(15.0f, 1.0f, 15.0f));
    scene3->addObject(new DrawableObject(models[2], nightShader, plainComp, glm::vec3(0.18f, 0.38f, 0.16f)));

    auto getRandomFloat = [](float minVal, float maxVal) -> float {
        return minVal + static_cast<float>(std::rand()) / (static_cast<float>(RAND_MAX / (maxVal - minVal)));
        };

    const int treeCount = 85;
    for (int i = 0; i < treeCount; ++i) {
        float posX = getRandomFloat(-13.5f, 13.5f);
        float posZ = getRandomFloat(-13.5f, 13.5f);

        if (std::abs(posX) < 1.5f && std::abs(posZ) < 1.5f) {
            posX += (posX >= 0.0f ? 2.5f : -2.5f);
        }

        if (glm::distance(glm::vec2(posX, posZ), glm::vec2(fireflyPos.x, fireflyPos.z)) < 0.8f) {
            posX += 1.5f;
        }

        float posY = -0.65f;
        float rotAngle = getRandomFloat(0.0f, 360.0f);
        float scaleVal = getRandomFloat(0.24f, 0.52f);

        auto treeComp = std::make_shared<CompositeTransformation>();
        treeComp->add(std::make_shared<Translation>(posX, posY, posZ));
        treeComp->add(std::make_shared<Rotation>(glm::radians(rotAngle), glm::vec3(0.0f, 1.0f, 0.0f)));
        treeComp->add(std::make_shared<Scale>(scaleVal));

        float greenShade = getRandomFloat(0.42f, 0.68f);
        float redShade = getRandomFloat(0.08f, 0.22f);
        float blueShade = getRandomFloat(0.10f, 0.20f);
        glm::vec3 treeFoliageColor(redShade, greenShade, blueShade);

        scene3->addObject(new DrawableObject(models[3], nightShader, treeComp, treeFoliageColor));
    }

    const int bushCount = 40;
    for (int i = 0; i < bushCount; ++i) {
        float posX = getRandomFloat(-13.0f, 13.0f);
        float posZ = getRandomFloat(-13.0f, 13.0f);
        float posY = -0.58f;
        float rotAngle = getRandomFloat(0.0f, 360.0f);
        float scaleVal = getRandomFloat(0.12f, 0.26f);

        auto bushComp = std::make_shared<CompositeTransformation>();
        bushComp->add(std::make_shared<Translation>(posX, posY, posZ));
        bushComp->add(std::make_shared<Rotation>(glm::radians(rotAngle), glm::vec3(0.0f, 1.0f, 0.0f)));
        bushComp->add(std::make_shared<Scale>(scaleVal));

        float greenBush = getRandomFloat(0.55f, 0.75f);
        glm::vec3 bushColor(0.15f, greenBush, 0.18f);

        scene3->addObject(new DrawableObject(models[4], nightShader, bushComp, bushColor));
    }

    auto forestClock1 = std::make_shared<CompositeTransformation>();
    forestClock1->add(std::make_shared<Translation>(-3.2f, 1.5f, 1.0f));
    forestClock1->add(std::make_shared<Rotation>(glm::radians(25.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
    forestClock1->add(std::make_shared<Scale>(1.1f, 1.1f, -1.1f));
    scene3->addObject(new DrawableObject(models[10], clockShader, forestClock1));

    auto forestClock2 = std::make_shared<CompositeTransformation>();
    forestClock2->add(std::make_shared<Translation>(3.5f, 2.3f, -2.5f));
    forestClock2->add(std::make_shared<Rotation>(glm::radians(-40.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
    forestClock2->add(std::make_shared<Scale>(1.6f, 1.6f, -1.6f));
    scene3->addObject(new DrawableObject(models[10], clockShader, forestClock2));

    auto fireflyComp = std::make_shared<CompositeTransformation>();
    fireflyComp->add(std::make_shared<Translation>(fireflyPos.x, fireflyPos.y, fireflyPos.z));
    fireflyComp->add(std::make_shared<Scale>(0.05f));
    scene3->addObject(new DrawableObject(models[1], shaders[2], fireflyComp, fireflyColor));

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
    scene5->addObject(new DrawableObject(models[8], blinnColorShader, earthComp));

    auto moonComp = std::make_shared<CompositeTransformation>();
    moonOrbitRotation = std::make_shared<Rotation>(0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    moonSelfRotation = std::make_shared<Rotation>(0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    moonComp->add(earthOrbitRotation);
    moonComp->add(std::make_shared<Translation>(7.0f, 0.0f, 0.0f));
    moonComp->add(moonOrbitRotation);
    moonComp->add(std::make_shared<Translation>(1.5f, 0.0f, 0.0f));
    moonComp->add(moonSelfRotation);
    moonComp->add(std::make_shared<Scale>(0.22f));
    scene5->addObject(new DrawableObject(models[9], blinnColorShader, moonComp));

    auto rocketComp = std::make_shared<CompositeTransformation>();
    rocketOrbitRotation = std::make_shared<Rotation>(0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    rocketSelfRotation = std::make_shared<Rotation>(0.0f, glm::vec3(0.0f, 1.0f, 0.0f));

    rocketComp->add(earthOrbitRotation);
    rocketComp->add(std::make_shared<Translation>(7.0f, 0.0f, 0.0f));
    rocketComp->add(rocketOrbitRotation);
    rocketComp->add(std::make_shared<Translation>(2.2f, 0.4f, 0.0f));
    rocketComp->add(rocketSelfRotation);
    rocketComp->add(std::make_shared<Scale>(0.12f));
    scene5->addObject(new DrawableObject(models[11], blinnColorShader, rocketComp));

    addWatermark(scene5);
    scenes.push_back(scene5);

    Scene* scene6 = new Scene("6: Clock");
    auto clockComp = std::make_shared<CompositeTransformation>();
    clockComp->add(std::make_shared<Scale>(1.15f, 1.15f, -1.15f));
    scene6->addObject(new DrawableObject(models[10], clockShader, clockComp));

    addWatermark(scene6);
    scenes.push_back(scene6);

    Scene* scene7 = new Scene("7: Lighting Models");

    auto topComp = std::make_shared<CompositeTransformation>();
    topComp->add(std::make_shared<Translation>(0.0f, 2.0f, 0.0f));
    topComp->add(std::make_shared<Scale>(0.65f));
    scene7->addObject(new DrawableObject(models[1], blinnShader, topComp));

    auto bottomComp = std::make_shared<CompositeTransformation>();
    bottomComp->add(std::make_shared<Translation>(0.0f, -2.0f, 0.0f));
    bottomComp->add(std::make_shared<Scale>(0.65f));
    scene7->addObject(new DrawableObject(models[1], blinnShader, bottomComp));

    auto leftComp = std::make_shared<CompositeTransformation>();
    leftComp->add(std::make_shared<Translation>(-2.0f, 0.0f, 0.0f));
    leftComp->add(std::make_shared<Scale>(0.65f));
    scene7->addObject(new DrawableObject(models[1], phongShader, leftComp));

    auto rightComp = std::make_shared<CompositeTransformation>();
    rightComp->add(std::make_shared<Translation>(2.0f, 0.0f, 0.0f));
    rightComp->add(std::make_shared<Scale>(0.65f));
    scene7->addObject(new DrawableObject(models[1], phongShader, rightComp));

    addWatermark(scene7);
    scenes.push_back(scene7);

    Scene* scene8 = new Scene("8: Shrek Scene");

    GLuint shrekTex = loadTexture("textures/shrek.png");
    GLuint fionaTex = loadTexture("textures/fiona.png");
    GLuint grassTex = loadTexture("textures/grass.png");

    std::vector<const char*> skyboxFaces = {
        "textures/posx.jpg",
        "textures/negx.jpg",
        "textures/posy.jpg",
        "textures/negy.jpg",
        "textures/posz.jpg",
        "textures/negz.jpg"
    };
    skyboxTex = loadCubemap(skyboxFaces);

    auto groundComp = std::make_shared<CompositeTransformation>();
    groundComp->add(std::make_shared<Translation>(0.0f, -1.0f, 0.0f));
    groundComp->add(std::make_shared<Scale>(15.0f, 1.0f, 15.0f));

    auto shrekComp = std::make_shared<CompositeTransformation>();
    shrekComp->add(std::make_shared<Translation>(0.0f, -1.0f, 0.0f));
    shrekComp->add(std::make_shared<Rotation>(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
    shrekComp->add(std::make_shared<Scale>(0.75f));

    auto fionaComp = std::make_shared<CompositeTransformation>();
    fionaComp->add(std::make_shared<Translation>(-2.0f, -1.0f, 0.0f));
    fionaComp->add(std::make_shared<Rotation>(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
    fionaComp->add(std::make_shared<Scale>(0.75f));

    scene8->addObject(new DrawableObject(models[12], textureShader, groundComp, glm::vec3(1.0f), grassTex));
    scene8->addObject(new DrawableObject(models[13], textureShader, shrekComp, glm::vec3(1.0f), shrekTex));
    scene8->addObject(new DrawableObject(models[14], textureShader, fionaComp, glm::vec3(1.0f), fionaTex));

    addWatermark(scene8);
    scenes.push_back(scene8);

    Scene* scene9 = new Scene("9: EB408 Model");

    std::vector<const char*> skyboxEB408 = {
        "textures/posx.jpg",
        "textures/negx.jpg",
        "textures/posy.jpg",
        "textures/negy.jpg",
        "textures/posz.jpg",
        "textures/negz.jpg"
    };
    skyboxTex = loadCubemap(skyboxEB408);

    auto ebComp = std::make_shared<CompositeTransformation>();
    ebComp->add(std::make_shared<Translation>(0.0f, 0.0f, 0.0f));
    ebComp->add(std::make_shared<Scale>(0.3f));

    auto doorFrameComp = std::make_shared<CompositeTransformation>();
    doorFrameComp->add(std::make_shared<Translation>(-0.73f, 0.0f, 2.35f));
    doorFrameComp->add(std::make_shared<Rotation>(glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
    doorFrameComp->add(std::make_shared<Scale>(0.003f));

    auto doorComp = std::make_shared<CompositeTransformation>();
    doorComp->add(std::make_shared<Translation>(-0.73f, 0.0f, 2.35f));
    doorComp->add(std::make_shared<Rotation>(glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
    doorComp->add(std::make_shared<Scale>(0.003f));

    auto wbComp = std::make_shared<CompositeTransformation>();
    wbComp->add(std::make_shared<Translation>(-1.1f, 0.085f, 0.85f));
    wbComp->add(std::make_shared<Rotation>(glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
    wbComp->add(std::make_shared<Scale>(0.3f));

    auto shrekebComp = std::make_shared<CompositeTransformation>();
    shrekebComp->add(std::make_shared<Translation>(-0.8f, 0.085f, 1.5f));
    shrekebComp->add(std::make_shared<Rotation>(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
    shrekebComp->add(std::make_shared<Rotation>(glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
    shrekebComp->add(std::make_shared<Scale>(0.3f));

    GLuint ebTex = loadTexture("textures/T_EB408.png");
    GLuint doorTex = loadTexture("textures/T_door_basecolor.png");
    GLuint wbTex = loadTexture("textures/T_Whiteboard_BaseColor.png");
    tableTex = loadTexture("textures/T_Table_BaseColor.png");
    ocTex = loadTexture("textures/T_OrangeChair_BaseColor.png");
    shrekebTex = loadTexture("textures/shrek.png");

    scene9->addObject(new DrawableObject(models[16], textureShader, ebComp, glm::vec3(1.0f), ebTex));
    scene9->addObject(new DrawableObject(models[17], textureShader, doorFrameComp, glm::vec3(1.0f), doorTex));
    scene9->addObject(new DrawableObject(models[18], textureShader, doorComp, glm::vec3(1.0f), doorTex));
    scene9->addObject(new DrawableObject(models[19], textureShader, wbComp, glm::vec3(1.0f), wbTex));
    scene9->addObject(new DrawableObject(models[13], textureShader, shrekebComp, glm::vec3(1.0f), shrekebTex));

    const int numRows = 3;
    const int tablesPerRow = 3;

    const float startX = -0.45f;
    const float startZ = 0.35f;

    const float stepX = 0.5f;
    const float stepZ = 0.45f;

    const float chairOffsetX = 0.18f;
    const float chairOffsetZ = 0.0f;

    for (int r = 0; r < numRows; ++r) {
        for (int c = 0; c < tablesPerRow; ++c) {
            float posX = startX + c * stepX;
            float posZ = startZ + r * stepZ;

            auto tableComp = std::make_shared<CompositeTransformation>();
            tableComp->add(std::make_shared<Translation>(posX, 0.0f, posZ));
            tableComp->add(std::make_shared<Rotation>(glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
            tableComp->add(std::make_shared<Rotation>(glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
            tableComp->add(std::make_shared<Scale>(0.3f));
            scene9->addObject(new DrawableObject(models[20], textureShader, tableComp, glm::vec3(1.0f), tableTex));

            auto ocComp = std::make_shared<CompositeTransformation>();
            ocComp->add(std::make_shared<Translation>(posX + chairOffsetX, 0.0f, posZ + chairOffsetZ));
            ocComp->add(std::make_shared<Rotation>(glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
            ocComp->add(std::make_shared<Rotation>(glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
            ocComp->add(std::make_shared<Scale>(0.3f));
            scene9->addObject(new DrawableObject(models[21], textureShader, ocComp, glm::vec3(1.0f), ocTex));
        }
    }

    addWatermark(scene9);
    scenes.push_back(scene9);
}

void Application::processInput() {
    float cameraSpeed = 3.0f * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera->moveForward(cameraSpeed);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera->moveBackward(cameraSpeed);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera->moveLeft(cameraSpeed);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera->moveRight(cameraSpeed);
}

void Application::run() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        float time = currentFrame;
        if (triRotLocal) triRotLocal->setAngle(time * 1.5f);
        if (triRotOrbit) triRotOrbit->setAngle(time * 1.5f);

        if (sunSelfRotation)    sunSelfRotation->setAngle(time * 0.35f);
        if (earthOrbitRotation) earthOrbitRotation->setAngle(time * 0.55f);
        if (earthSelfRotation)  earthSelfRotation->setAngle(time * 2.2f);
        if (moonOrbitRotation)  moonOrbitRotation->setAngle(time * 3.2f);
        if (moonSelfRotation)   moonSelfRotation->setAngle(time * 1.5f);

        if (rocketOrbitRotation) rocketOrbitRotation->setAngle(time * 2.8f);
        if (rocketSelfRotation)  rocketSelfRotation->setAngle(time * 1.8f);

        if (clockShader) {
            std::time_t rawTime = std::time(nullptr);
            std::tm timeInfo;
            localtime_s(&timeInfo, &rawTime);

            float seconds = static_cast<float>(timeInfo.tm_sec);
            float minutes = static_cast<float>(timeInfo.tm_min) + seconds / 60.0f;
            float hours = static_cast<float>(timeInfo.tm_hour % 12) + minutes / 60.0f;

            float secAngle = -(seconds / 60.0f) * glm::two_pi<float>();
            float minAngle = -(minutes / 60.0f) * glm::two_pi<float>();
            float hourAngle = -(hours / 12.0f) * glm::two_pi<float>();

            clockShader->use();
            clockShader->setUniform("uSecondAngle", secAngle);
            clockShader->setUniform("uMinuteAngle", minAngle);
            clockShader->setUniform("uHourAngle", hourAngle);
        }

        if (activeSceneIndex == 2 && nightShader != nullptr) {
            nightShader->use();

            nightShader->setUniform("lightPosition", glm::vec3(0.0f, 0.8f, 0.0f));
            nightShader->setUniform("lightColor", glm::vec3(0.9f, 1.0f, 0.3f) * 2.5f);
            nightShader->setUniform("cameraPosition", camera->getPosition());

            nightShader->setUniform("constantAttenuation", 1.0f);
            nightShader->setUniform("linearAttenuation", 0.09f);
            nightShader->setUniform("quadraticAttenuation", 0.032f);
        } 
        else if (activeSceneIndex == 7 && nightShader != nullptr) {
            nightShader->use();

            nightShader->setUniform("lightPosition", glm::vec3(2.0f, 8.0f, 6.0f));
            nightShader->setUniform("lightColor", glm::vec3(1.0f, 1.0f, 0.95f) * 2.2f);
            nightShader->setUniform("cameraPosition", camera->getPosition());

            nightShader->setUniform("constantAttenuation", 1.0f);
            nightShader->setUniform("linearAttenuation", 0.05f);
            nightShader->setUniform("quadraticAttenuation", 0.01f);
        }

        if ((activeSceneIndex == 7 || activeSceneIndex == 8) && skyboxShader != nullptr) {
            glDepthFunc(GL_LEQUAL);
            glStencilFunc(GL_ALWAYS, 0, 0xFF);

            skyboxShader->use();
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTex);
            skyboxShader->setUniform("skybox", 0);

            models[15]->draw();

            glDepthFunc(GL_LESS);
        }

        for (MovingObject& mObj : movingObjects) {
            if (mObj.sceneIndex != activeSceneIndex || !mObj.translation) continue;

            float dist = glm::distance(mObj.pointA, mObj.pointB);
            float step = (dist > 0.0001f) ? (mObj.speed * deltaTime / dist) : 0.0f;
            mObj.t += static_cast<float>(mObj.direction) * step;

            if (mObj.t >= 1.0f) {
                mObj.t = 1.0f;
                mObj.direction = -1;
            }
            else if (mObj.t <= 0.0f) {
                mObj.t = 0.0f;
                mObj.direction = 1;
            }

            glm::vec3 pos = mObj.pointA + mObj.t * (mObj.pointB - mObj.pointA);
            mObj.translation->setPosition(pos);
        }

        if (activeSceneIndex < scenes.size() && scenes[activeSceneIndex] != nullptr) {
            scenes[activeSceneIndex]->render();
        }

        performPicking();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void Application::handleKeyboard(int key, int action) {
    if (action == GLFW_PRESS) {
        if (key == GLFW_KEY_ESCAPE) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }
        else if (key >= GLFW_KEY_1 && key <= GLFW_KEY_9) {
            size_t idx = static_cast<size_t>(key - GLFW_KEY_1);
            if (camera && idx < scenes.size()) {
                activeSceneIndex = idx;
                std::cout << "Switched to scene: " << scenes[idx]->getName() << std::endl;
                if (idx == 5) {
                    camera->setPosition(glm::vec3(0.0f, 0.0f, 2.35f));
                    camera->setOrientation(glm::radians(90.0f), glm::radians(-90.0f));
                }
                else if (idx == 2) {
                    camera->setPosition(glm::vec3(0.0f, 1.5f, 5.0f));
                    camera->setOrientation(glm::radians(90.0f), glm::radians(-90.0f));
                }
                else if (idx == 4) {
                    camera->setPosition(glm::vec3(0.0f, 5.0f, 15.0f));
                    camera->setOrientation(glm::radians(90.0f), glm::radians(-90.0f));
                }
                else {
                    camera->setPosition(glm::vec3(0.0f, 0.0f, 2.5f));
                    camera->setOrientation(glm::radians(90.0f), glm::radians(-90.0f));
                }
            }
        }
        else if (key == GLFW_KEY_P) {
            camera->setProjectionType(ProjectionType::Perspective);
            std::cout << "Change projection: Perspective" << std::endl;
        }
        else if (key == GLFW_KEY_O) {
            camera->setProjectionType(ProjectionType::Orthogonal);
            std::cout << "Change projection: Orthogonal" << std::endl;
        }
        else if (key == GLFW_KEY_I) {
            interactionMode = InteractionMode::Select;
            hasFirstLinePoint = false;
            std::cout << "Mode: Select / Delete" << std::endl;
        }
        else if (key == GLFW_KEY_B) {
            interactionMode = InteractionMode::PlaceStatic;
            hasFirstLinePoint = false;
            std::cout << "Mode: Place static object" << std::endl;
        }
        else if (key == GLFW_KEY_M) {
            interactionMode = InteractionMode::PlaceMoving;
            hasFirstLinePoint = false;
            std::cout << "Mode: Place moving model" << std::endl;
        }
    }
}