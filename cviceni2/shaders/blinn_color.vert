#version 330 core

// Атрибуты планет и ракеты при hasColor == true
layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inColor;
layout (location = 2) in vec3 inNormal;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec3 worldPosition;
out vec3 worldNormal;
out vec3 fragColor;

void main(void)
{
    vec4 wp = modelMatrix * vec4(inPosition, 1.0);
    worldPosition = wp.xyz / wp.w;

    mat3 normalMatrix = mat3(transpose(inverse(modelMatrix)));
    worldNormal = normalMatrix * inNormal;

    fragColor = inColor;

    gl_Position = projectionMatrix * viewMatrix * wp;
}