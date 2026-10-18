#version 330 core

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inNormal;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectMatrix;

uniform vec3 uColor;

out vec3 fragColor;
out vec3 fragNormal;
flat out int isLeaf;

void main()
{
    gl_Position = projectMatrix * viewMatrix * modelMatrix * vec4(inPosition, 1.0);

    if (gl_VertexID > 36443) {
        fragColor = uColor;
        isLeaf = 1;
    } else {
        fragColor = vec3(0.42, 0.26, 0.15);
        isLeaf = 0;
    }

    fragNormal = mat3(transpose(inverse(modelMatrix))) * inNormal;
}