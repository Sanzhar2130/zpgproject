#include "LightController.h"
#include <glm/glm.hpp>

void LightController::update(size_t sceneIndex, ShaderProgram* nightShader, Camera* camera) {
    if (!nightShader || !camera) return;

    if (sceneIndex == 2) {
        nightShader->use();
        nightShader->setUniform("lightPosition", glm::vec3(0.0f, 0.8f, 0.0f));
        nightShader->setUniform("lightColor", glm::vec3(0.9f, 1.0f, 0.3f) * 2.5f);
        nightShader->setUniform("cameraPosition", camera->getPosition());
        nightShader->setUniform("constantAttenuation", 1.0f);
        nightShader->setUniform("linearAttenuation", 0.09f);
        nightShader->setUniform("quadraticAttenuation", 0.032f);
    }
    else if (sceneIndex == 7) {
        nightShader->use();
        nightShader->setUniform("lightPosition", glm::vec3(2.0f, 8.0f, 6.0f));
        nightShader->setUniform("lightColor", glm::vec3(1.0f, 1.0f, 0.95f) * 2.2f);
        nightShader->setUniform("cameraPosition", camera->getPosition());
        nightShader->setUniform("constantAttenuation", 1.0f);
        nightShader->setUniform("linearAttenuation", 0.05f);
        nightShader->setUniform("quadraticAttenuation", 0.01f);
    }
}