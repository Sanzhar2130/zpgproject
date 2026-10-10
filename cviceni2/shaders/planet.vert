#version 330 core

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inColor;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectMatrix;

out vec3 fragColor;

void main()
{
    gl_Position = projectMatrix * viewMatrix * modelMatrix * vec4(inPosition, 1.0);
    fragColor = inColor;
}