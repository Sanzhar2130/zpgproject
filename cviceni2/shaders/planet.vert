#version 330 core

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inColor;
layout (location = 2) in vec3 inNormal;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectMatrix;

out vec3 fragColor;
out vec3 fragNormal;

void main()
{
    fragColor = inColor;
    fragNormal = mat3(transpose(inverse(modelMatrix))) * inNormal;
    gl_Position = projectMatrix * viewMatrix * modelMatrix * vec4(inPosition, 1.0);
}