/**
 * @file ShaderFactory.cpp
 * @author Student (login: ZHA0067)
 * @brief Implementation of ShaderFactory creating default shader programs.
 */
#include "ShaderFactory.h"
#include "Shader.h"

ShaderBundle ShaderFactory::createDefaultShaders() {
    ShaderBundle bundle;
    bundle.allShaders.reserve(13);

    Shader vertColor(GL_VERTEX_SHADER, "shaders/vertex_color.vert");
    Shader fragColor(GL_FRAGMENT_SHADER, "shaders/vertex_color.frag");
    ShaderProgram* spColor = new ShaderProgram(vertColor, fragColor);
    bundle.allShaders.push_back(spColor);

    Shader vertNormal(GL_VERTEX_SHADER, "shaders/transform.vert");
    Shader fragAbsNormal(GL_FRAGMENT_SHADER, "shaders/abs_normals.frag");
    ShaderProgram* spNormal = new ShaderProgram(vertNormal, fragAbsNormal);
    bundle.allShaders.push_back(spNormal);

    Shader fragUniformColor(GL_FRAGMENT_SHADER, "shaders/color_uniform.frag");
    ShaderProgram* spUniform = new ShaderProgram(vertNormal, fragUniformColor);
    bundle.allShaders.push_back(spUniform);

    Shader vertClock(GL_VERTEX_SHADER, "shaders/clock.vert");
    Shader fragClock(GL_FRAGMENT_SHADER, "shaders/clock.frag");
    bundle.clockShader = new ShaderProgram(vertClock, fragClock);
    bundle.allShaders.push_back(bundle.clockShader);

    Shader vertTree(GL_VERTEX_SHADER, "shaders/tree.vert");
    Shader fragTree(GL_FRAGMENT_SHADER, "shaders/tree.frag");
    bundle.treeShader = new ShaderProgram(vertTree, fragTree);
    bundle.allShaders.push_back(bundle.treeShader);

    Shader vertAmbient(GL_VERTEX_SHADER, "shaders/ambient.vert");
    Shader fragAmbient(GL_FRAGMENT_SHADER, "shaders/ambient.frag");
    bundle.ambientShader = new ShaderProgram(vertAmbient, fragAmbient);
    bundle.allShaders.push_back(bundle.ambientShader);

    Shader vertLambert(GL_VERTEX_SHADER, "shaders/lambert.vert");
    Shader fragLambert(GL_FRAGMENT_SHADER, "shaders/lambert.frag");
    ShaderProgram* spLambert = new ShaderProgram(vertLambert, fragLambert);
    bundle.allShaders.push_back(spLambert);

    Shader vertPhong(GL_VERTEX_SHADER, "shaders/phong.vert");
    Shader fragPhong(GL_FRAGMENT_SHADER, "shaders/phong.frag");
    bundle.phongShader = new ShaderProgram(vertPhong, fragPhong);
    bundle.allShaders.push_back(bundle.phongShader);

    Shader vertBlinn(GL_VERTEX_SHADER, "shaders/blinn.vert");
    Shader fragBlinn(GL_FRAGMENT_SHADER, "shaders/blinn.frag");
    bundle.blinnShader = new ShaderProgram(vertBlinn, fragBlinn);
    bundle.allShaders.push_back(bundle.blinnShader);

    Shader vertBlinnColor(GL_VERTEX_SHADER, "shaders/blinn_color.vert");
    Shader fragBlinnColor(GL_FRAGMENT_SHADER, "shaders/blinn_color.frag");
    bundle.blinnColorShader = new ShaderProgram(vertBlinnColor, fragBlinnColor);
    bundle.allShaders.push_back(bundle.blinnColorShader);

    Shader vertNight(GL_VERTEX_SHADER, "shaders/night_forest.vert");
    Shader fragNight(GL_FRAGMENT_SHADER, "shaders/night_forest.frag");
    bundle.nightShader = new ShaderProgram(vertNight, fragNight);
    bundle.allShaders.push_back(bundle.nightShader);

    Shader vertTex(GL_VERTEX_SHADER, "shaders/texture.vert");
    Shader fragTex(GL_FRAGMENT_SHADER, "shaders/texture.frag");
    bundle.textureShader = new ShaderProgram(vertTex, fragTex);
    bundle.allShaders.push_back(bundle.textureShader);

    Shader vertSkybox(GL_VERTEX_SHADER, "shaders/skybox.vert");
    Shader fragSkybox(GL_FRAGMENT_SHADER, "shaders/skybox.frag");
    bundle.skyboxShader = new ShaderProgram(vertSkybox, fragSkybox);
    bundle.allShaders.push_back(bundle.skyboxShader);

    return bundle;
}