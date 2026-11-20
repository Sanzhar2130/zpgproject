#include "SkyboxRenderer.h"

void SkyboxRenderer::render(size_t sceneIndex, ShaderProgram* skyboxShader, GLuint skyboxTex, Model* skyboxModel) {
    if ((sceneIndex != 7 && sceneIndex != 8) || !skyboxShader || !skyboxModel) return;

    glDepthFunc(GL_LEQUAL);
    glStencilFunc(GL_ALWAYS, 0, 0xFF);

    skyboxShader->use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTex);
    skyboxShader->setUniform("skybox", 0);

    skyboxModel->draw();

    glDepthFunc(GL_LESS);
}