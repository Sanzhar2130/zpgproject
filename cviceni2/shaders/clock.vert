#version 330 core

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec3 inColor;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectMatrix;

uniform float uHourAngle;
uniform float uMinuteAngle;
uniform float uSecondAngle;

out vec3 fragColor;
out vec3 fragNormal;

vec2 rotate2D(vec2 pos, float angle) {
    float c = cos(angle);
    float s = sin(angle);
    return vec2(pos.x * c - pos.y * s, pos.x * s + pos.y * c);
}

void main()
{
    vec3 animatedPos = inPosition;

    if (inColor.r > 0.8 && inColor.g < 0.2 && inColor.b < 0.2) {
        animatedPos.xy = rotate2D(animatedPos.xy, uSecondAngle);
    }
    else if (inColor.g > 0.8 && inColor.r < 0.2 && inColor.b < 0.2) {
        animatedPos.xy = rotate2D(animatedPos.xy, uMinuteAngle);
    }
    else if (inColor.b > 0.8 && inColor.r < 0.2 && inColor.g < 0.2) {
        animatedPos.xy = rotate2D(animatedPos.xy, uHourAngle);
    }

    gl_Position = projectMatrix * viewMatrix * modelMatrix * vec4(animatedPos, 1.0);

    fragColor = inColor;
    fragNormal = mat3(transpose(inverse(modelMatrix))) * inNormal;
}