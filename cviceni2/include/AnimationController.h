#pragma once

#include <memory>
#include "ShaderProgram.h"
#include "Rotation.h"

struct SceneAnimRefs {
    std::shared_ptr<Rotation> triRotLocal;
    std::shared_ptr<Rotation> triRotOrbit;
    std::shared_ptr<Rotation> sunSelfRotation;
    std::shared_ptr<Rotation> earthOrbitRotation;
    std::shared_ptr<Rotation> earthSelfRotation;
    std::shared_ptr<Rotation> moonOrbitRotation;
    std::shared_ptr<Rotation> moonSelfRotation;
    std::shared_ptr<Rotation> rocketOrbitRotation;
    std::shared_ptr<Rotation> rocketSelfRotation;
};

class AnimationController {
public:
    void update(float time, const SceneAnimRefs& anim, ShaderProgram* clockShader);
};