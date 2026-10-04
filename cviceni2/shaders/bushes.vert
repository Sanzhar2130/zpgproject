#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;

out vec3 vertexColor;

void main() 
{    
    vertexColor = color;
    gl_Position = vec4(position * 0.5f + vec3(0.0f, -0.5f, 0.0f), 1.0f);

}