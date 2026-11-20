/**
 * @file SceneFactory.cpp
 * @author Student (login: ZHA0067)
 * @brief Implementation of SceneFactory building scenes 1..9.
 */
#include "SceneFactory.h"
#include "TextureLoader.h"
#include "Translation.h"
#include "Scale.h"
#include "CompositeTransformation.h"

#include <glm/gtc/matrix_transform.hpp>
#include <cstdlib>
#include <cmath>
#include <string>

namespace {
    float randomFloat(float minVal, float maxVal) {
        return minVal + static_cast<float>(std::rand()) /
            (static_cast<float>(RAND_MAX) / (maxVal - minVal));
    }

    std::shared_ptr<CompositeTransformation> createTableTransform(float x, float y, float z, float uniformScale = 0.3f) {
        auto comp = std::make_shared<CompositeTransformation>();
        comp->add(std::make_shared<Translation>(x, y, z));
        comp->add(std::make_shared<Rotation>(glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
        comp->add(std::make_shared<Rotation>(glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
        comp->add(std::make_shared<Scale>(uniformScale));
        return comp;
    }

    std::shared_ptr<CompositeTransformation> createChairTransform(float x, float y, float z, float uniformScale = 0.3f) {
        auto comp = std::make_shared<CompositeTransformation>();
        comp->add(std::make_shared<Translation>(x, y, z));
        comp->add(std::make_shared<Rotation>(glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
        comp->add(std::make_shared<Rotation>(glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
        comp->add(std::make_shared<Scale>(uniformScale));
        return comp;
    }
}

void SceneFactory::addWatermark(Scene* scene, Model* watermarkModel, ShaderProgram* uniformColorShader) {
    if (!scene || !watermarkModel || !uniformColorShader) return;

    auto wmComp = std::make_shared<CompositeTransformation>();
    const std::string& name = scene->getName();

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

    scene->addObject(new DrawableObject(watermarkModel, uniformColorShader, wmComp, glm::vec3(0.0f, 0.0f, 1.0f)));
}

SceneBuildResult SceneFactory::createDefaultScenes(const SceneBuildContext& ctx) {
    SceneBuildResult res;
    const auto& models = ctx.models;
    const auto& shaders = ctx.shaders;

    ShaderProgram* spColor = (shaders.size() > 0) ? shaders[0] : nullptr;
    ShaderProgram* spNormal = (shaders.size() > 1) ? shaders[1] : nullptr;
    ShaderProgram* spUniform = (shaders.size() > 2) ? shaders[2] : nullptr;

    Model* wmModel = (models.size() > 6) ? models[6] : nullptr;

    // 1: Triangle
    Scene* scene1 = new Scene("1: Triangle");

    auto comp1 = std::make_shared<CompositeTransformation>();
    comp1->add(std::make_shared<Translation>(-0.85f, 0.0f, 0.0f));
    res.triRotLocal = std::make_shared<Rotation>(0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
    comp1->add(res.triRotLocal);
    comp1->add(std::make_shared<Scale>(0.65f));
    scene1->addObject(new DrawableObject(models[0], spColor, comp1));

    auto comp2 = std::make_shared<CompositeTransformation>();
    res.triRotOrbit = std::make_shared<Rotation>(0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
    comp2->add(res.triRotOrbit);
    comp2->add(std::make_shared<Translation>(0.85f, 0.0f, 0.0f));
    comp2->add(std::make_shared<Scale>(0.65f));
    scene1->addObject(new DrawableObject(models[0], spColor, comp2));

    addWatermark(scene1, wmModel, spUniform);
    res.scenes.push_back(scene1);

    // 2: Sphere
    Scene* scene2 = new Scene("2: Sphere");
    auto sphereComp = std::make_shared<CompositeTransformation>();
    sphereComp->add(std::make_shared<Scale>(0.85f));
    scene2->addObject(new DrawableObject(models[1], spNormal, sphereComp));
    addWatermark(scene2, wmModel, spUniform);
    res.scenes.push_back(scene2);

    // 3: Forest
    Scene* scene3 = new Scene("3: Forest");
    glm::vec3 fireflyPos(0.0f, 0.8f, 0.0f);
    glm::vec3 fireflyColor(0.9f, 1.0f, 0.3f);

    auto plainComp = std::make_shared<CompositeTransformation>();
    plainComp->add(std::make_shared<Translation>(0.0f, -0.65f, 0.0f));
    plainComp->add(std::make_shared<Scale>(15.0f, 1.0f, 15.0f));
    scene3->addObject(new DrawableObject(models[2], ctx.nightShader, plainComp, glm::vec3(0.18f, 0.38f, 0.16f)));

    const int treeCount = 85;
    for (int i = 0; i < treeCount; ++i) {
        float posX = randomFloat(-13.5f, 13.5f);
        float posZ = randomFloat(-13.5f, 13.5f);

        if (std::abs(posX) < 1.5f && std::abs(posZ) < 1.5f) {
            posX += (posX >= 0.0f ? 2.5f : -2.5f);
        }

        if (glm::distance(glm::vec2(posX, posZ), glm::vec2(fireflyPos.x, fireflyPos.z)) < 0.8f) {
            posX += 1.5f;
        }

        float posY = -0.65f;
        float rotAngle = randomFloat(0.0f, 360.0f);
        float scaleVal = randomFloat(0.24f, 0.52f);

        auto treeComp = std::make_shared<CompositeTransformation>();
        treeComp->add(std::make_shared<Translation>(posX, posY, posZ));
        treeComp->add(std::make_shared<Rotation>(glm::radians(rotAngle), glm::vec3(0.0f, 1.0f, 0.0f)));
        treeComp->add(std::make_shared<Scale>(scaleVal));

        glm::vec3 treeFoliageColor(
            randomFloat(0.08f, 0.22f),
            randomFloat(0.42f, 0.68f),
            randomFloat(0.10f, 0.20f)
        );

        scene3->addObject(new DrawableObject(models[3], ctx.nightShader, treeComp, treeFoliageColor));
    }

    const int bushCount = 40;
    for (int i = 0; i < bushCount; ++i) {
        float posX = randomFloat(-13.0f, 13.0f);
        float posZ = randomFloat(-13.0f, 13.0f);
        float posY = -0.58f;
        float rotAngle = randomFloat(0.0f, 360.0f);
        float scaleVal = randomFloat(0.12f, 0.26f);

        auto bushComp = std::make_shared<CompositeTransformation>();
        bushComp->add(std::make_shared<Translation>(posX, posY, posZ));
        bushComp->add(std::make_shared<Rotation>(glm::radians(rotAngle), glm::vec3(0.0f, 1.0f, 0.0f)));
        bushComp->add(std::make_shared<Scale>(scaleVal));

        glm::vec3 bushColor(0.15f, randomFloat(0.55f, 0.75f), 0.18f);

        scene3->addObject(new DrawableObject(models[4], ctx.nightShader, bushComp, bushColor));
    }

    auto forestClock1 = std::make_shared<CompositeTransformation>();
    forestClock1->add(std::make_shared<Translation>(-3.2f, 1.5f, 1.0f));
    forestClock1->add(std::make_shared<Rotation>(glm::radians(25.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
    forestClock1->add(std::make_shared<Scale>(1.1f, 1.1f, -1.1f));
    scene3->addObject(new DrawableObject(models[10], ctx.clockShader, forestClock1));

    auto forestClock2 = std::make_shared<CompositeTransformation>();
    forestClock2->add(std::make_shared<Translation>(3.5f, 2.3f, -2.5f));
    forestClock2->add(std::make_shared<Rotation>(glm::radians(-40.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
    forestClock2->add(std::make_shared<Scale>(1.6f, 1.6f, -1.6f));
    scene3->addObject(new DrawableObject(models[10], ctx.clockShader, forestClock2));

    auto fireflyComp = std::make_shared<CompositeTransformation>();
    fireflyComp->add(std::make_shared<Translation>(fireflyPos.x, fireflyPos.y, fireflyPos.z));
    fireflyComp->add(std::make_shared<Scale>(0.05f));
    scene3->addObject(new DrawableObject(models[1], spUniform, fireflyComp, fireflyColor));

    addWatermark(scene3, wmModel, spUniform);
    res.scenes.push_back(scene3);

    // 4: Model ZHA0067
    Scene* scene4 = new Scene("4: Model ZHA0067");
    auto loginComp = std::make_shared<CompositeTransformation>();
    loginComp->add(std::make_shared<Scale>(0.85f));
    scene4->addObject(new DrawableObject(models[6], spNormal, loginComp));
    res.scenes.push_back(scene4);

    // 5: Solar System
    Scene* scene5 = new Scene("5: Solar System");

    auto sunComp = std::make_shared<CompositeTransformation>();
    res.sunSelfRotation = std::make_shared<Rotation>(0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    sunComp->add(res.sunSelfRotation);
    sunComp->add(std::make_shared<Scale>(1.6f));
    scene5->addObject(new DrawableObject(models[7], spColor, sunComp));

    auto earthComp = std::make_shared<CompositeTransformation>();
    res.earthOrbitRotation = std::make_shared<Rotation>(0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    res.earthSelfRotation = std::make_shared<Rotation>(0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    earthComp->add(res.earthOrbitRotation);
    earthComp->add(std::make_shared<Translation>(7.0f, 0.0f, 0.0f));
    earthComp->add(res.earthSelfRotation);
    earthComp->add(std::make_shared<Scale>(0.6f));
    scene5->addObject(new DrawableObject(models[8], ctx.blinnColorShader, earthComp));

    auto moonComp = std::make_shared<CompositeTransformation>();
    res.moonOrbitRotation = std::make_shared<Rotation>(0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    res.moonSelfRotation = std::make_shared<Rotation>(0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    moonComp->add(res.earthOrbitRotation);
    moonComp->add(std::make_shared<Translation>(7.0f, 0.0f, 0.0f));
    moonComp->add(res.moonOrbitRotation);
    moonComp->add(std::make_shared<Translation>(1.5f, 0.0f, 0.0f));
    moonComp->add(res.moonSelfRotation);
    moonComp->add(std::make_shared<Scale>(0.22f));
    scene5->addObject(new DrawableObject(models[9], ctx.blinnColorShader, moonComp));

    auto rocketComp = std::make_shared<CompositeTransformation>();
    res.rocketOrbitRotation = std::make_shared<Rotation>(0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    res.rocketSelfRotation = std::make_shared<Rotation>(0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    rocketComp->add(res.earthOrbitRotation);
    rocketComp->add(std::make_shared<Translation>(7.0f, 0.0f, 0.0f));
    rocketComp->add(res.rocketOrbitRotation);
    rocketComp->add(std::make_shared<Translation>(2.2f, 0.4f, 0.0f));
    rocketComp->add(res.rocketSelfRotation);
    rocketComp->add(std::make_shared<Scale>(0.12f));
    scene5->addObject(new DrawableObject(models[11], ctx.blinnColorShader, rocketComp));

    addWatermark(scene5, wmModel, spUniform);
    res.scenes.push_back(scene5);

    // 6: Clock
    Scene* scene6 = new Scene("6: Clock");
    auto clockComp = std::make_shared<CompositeTransformation>();
    clockComp->add(std::make_shared<Scale>(1.15f, 1.15f, -1.15f));
    scene6->addObject(new DrawableObject(models[10], ctx.clockShader, clockComp));
    addWatermark(scene6, wmModel, spUniform);
    res.scenes.push_back(scene6);

    // 7: Lighting Models
    Scene* scene7 = new Scene("7: Lighting Models");

    auto topComp = std::make_shared<CompositeTransformation>();
    topComp->add(std::make_shared<Translation>(0.0f, 2.0f, 0.0f));
    topComp->add(std::make_shared<Scale>(0.65f));
    scene7->addObject(new DrawableObject(models[1], ctx.blinnShader, topComp));

    auto bottomComp = std::make_shared<CompositeTransformation>();
    bottomComp->add(std::make_shared<Translation>(0.0f, -2.0f, 0.0f));
    bottomComp->add(std::make_shared<Scale>(0.65f));
    scene7->addObject(new DrawableObject(models[1], ctx.blinnShader, bottomComp));

    auto leftComp = std::make_shared<CompositeTransformation>();
    leftComp->add(std::make_shared<Translation>(-2.0f, 0.0f, 0.0f));
    leftComp->add(std::make_shared<Scale>(0.65f));
    scene7->addObject(new DrawableObject(models[1], ctx.phongShader, leftComp));

    auto rightComp = std::make_shared<CompositeTransformation>();
    rightComp->add(std::make_shared<Translation>(2.0f, 0.0f, 0.0f));
    rightComp->add(std::make_shared<Scale>(0.65f));
    scene7->addObject(new DrawableObject(models[1], ctx.phongShader, rightComp));

    addWatermark(scene7, wmModel, spUniform);
    res.scenes.push_back(scene7);

    // 8: Shrek Scene
    Scene* scene8 = new Scene("8: Shrek Scene");

    res.shrekTex = loadTexture("textures/shrek.png");
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
    res.skyboxTex = loadCubemap(skyboxFaces);

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

    scene8->addObject(new DrawableObject(models[12], ctx.textureShader, groundComp, glm::vec3(1.0f), grassTex));
    scene8->addObject(new DrawableObject(models[13], ctx.textureShader, shrekComp, glm::vec3(1.0f), res.shrekTex));
    scene8->addObject(new DrawableObject(models[14], ctx.textureShader, fionaComp, glm::vec3(1.0f), fionaTex));

    addWatermark(scene8, wmModel, spUniform);
    res.scenes.push_back(scene8);

    // 9: EB408 Model
    Scene* scene9 = new Scene("9: EB408 Model");

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
    res.tableTex = loadTexture("textures/T_Table_BaseColor.png");
    res.ocTex = loadTexture("textures/T_OrangeChair_BaseColor.png");
    res.shrekebTex = loadTexture("textures/shrek.png");

    scene9->addObject(new DrawableObject(models[16], ctx.textureShader, ebComp, glm::vec3(1.0f), ebTex));
    scene9->addObject(new DrawableObject(models[17], ctx.textureShader, doorFrameComp, glm::vec3(1.0f), doorTex));
    scene9->addObject(new DrawableObject(models[18], ctx.textureShader, doorComp, glm::vec3(1.0f), doorTex));
    scene9->addObject(new DrawableObject(models[19], ctx.textureShader, wbComp, glm::vec3(1.0f), wbTex));
    scene9->addObject(new DrawableObject(models[13], ctx.textureShader, shrekebComp, glm::vec3(1.0f), res.shrekebTex));

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

            scene9->addObject(new DrawableObject(
                models[20], ctx.textureShader, createTableTransform(posX, 0.0f, posZ), glm::vec3(1.0f), res.tableTex));

            scene9->addObject(new DrawableObject(
                models[21], ctx.textureShader, createChairTransform(posX + chairOffsetX, 0.0f, posZ + chairOffsetZ), glm::vec3(1.0f), res.ocTex));
        }
    }

    addWatermark(scene9, wmModel, spUniform);
    res.scenes.push_back(scene9);

    return res;
}