#include "AnimationController.h"
#include <ctime>
#include <glm/gtc/constants.hpp>

void AnimationController::update(float time, const SceneAnimRefs& anim, ShaderProgram* clockShader) {
    if (anim.triRotLocal) anim.triRotLocal->setAngle(time * 1.5f);
    if (anim.triRotOrbit) anim.triRotOrbit->setAngle(time * 1.5f);

    if (anim.sunSelfRotation)     anim.sunSelfRotation->setAngle(time * 0.35f);
    if (anim.earthOrbitRotation)  anim.earthOrbitRotation->setAngle(time * 0.55f);
    if (anim.earthSelfRotation)   anim.earthSelfRotation->setAngle(time * 2.2f);
    if (anim.moonOrbitRotation)   anim.moonOrbitRotation->setAngle(time * 3.2f);
    if (anim.moonSelfRotation)    anim.moonSelfRotation->setAngle(time * 1.5f);
    if (anim.rocketOrbitRotation) anim.rocketOrbitRotation->setAngle(time * 2.8f);
    if (anim.rocketSelfRotation)  anim.rocketSelfRotation->setAngle(time * 1.8f);

    if (clockShader) {
        std::time_t rawTime = std::time(nullptr);
        std::tm timeInfo;
        localtime_s(&timeInfo, &rawTime);

        float seconds = static_cast<float>(timeInfo.tm_sec);
        float minutes = static_cast<float>(timeInfo.tm_min) + seconds / 60.0f;
        float hours = static_cast<float>(timeInfo.tm_hour % 12) + minutes / 60.0f;

        clockShader->use();
        clockShader->setUniform("uSecondAngle", -(seconds / 60.0f) * glm::two_pi<float>());
        clockShader->setUniform("uMinuteAngle", -(minutes / 60.0f) * glm::two_pi<float>());
        clockShader->setUniform("uHourAngle", -(hours / 12.0f) * glm::two_pi<float>());
    }
}