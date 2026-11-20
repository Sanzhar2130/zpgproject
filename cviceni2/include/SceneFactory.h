/**
 * @file SceneFactory.h
 * @author Student (login: ZHA0067)
 * @brief Factory for building all application scenes.
 */
#pragma once

#include <vector>
#include <memory>
#include <glad/gl.h>

#include "Scene.h"
#include "Model.h"
#include "ShaderProgram.h"
#include "Rotation.h"

struct SceneBuildContext {
    const std::vector<Model*>& models;
    const std::vector<ShaderProgram*>& shaders;

    ShaderProgram* clockShader = nullptr;
    ShaderProgram* blinnShader = nullptr;
    ShaderProgram* phongShader = nullptr;
    ShaderProgram* blinnColorShader = nullptr;
    ShaderProgram* nightShader = nullptr;
    ShaderProgram* textureShader = nullptr;
};

struct SceneBuildResult {
    std::vector<Scene*> scenes;

    std::shared_ptr<Rotation> triRotLocal;
    std::shared_ptr<Rotation> triRotOrbit;

    std::shared_ptr<Rotation> sunSelfRotation;
    std::shared_ptr<Rotation> earthOrbitRotation;
    std::shared_ptr<Rotation> earthSelfRotation;
    std::shared_ptr<Rotation> moonOrbitRotation;
    std::shared_ptr<Rotation> moonSelfRotation;

    std::shared_ptr<Rotation> rocketOrbitRotation;
    std::shared_ptr<Rotation> rocketSelfRotation;

    GLuint skyboxTex = 0;
    GLuint shrekTex = 0;
    GLuint tableTex = 0;
    GLuint ocTex = 0;
    GLuint shrekebTex = 0;
};

class SceneFactory {
public:
    static SceneBuildResult createDefaultScenes(const SceneBuildContext& ctx);

private:
    static void addWatermark(Scene* scene, Model* watermarkModel, ShaderProgram* uniformColorShader);
};