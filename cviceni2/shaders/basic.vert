#version 330 core

layout (location = 0) in vec3 inPosition;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectMatrix;

void main()
{
    gl_Position = projectMatrix * viewMatrix * modelMatrix * vec4(inPosition, 1.0);
}