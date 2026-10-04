#version 330 core

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inNormal;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectMatrix;

out vec3 fragNormal;
out vec3 fragWorldPos;
out vec3 fragColor;

void main()
{
    vec4 worldPos = modelMatrix * vec4(inPosition, 1.0);
    fragWorldPos = worldPos.xyz;

    fragNormal = mat3(transpose(inverse(modelMatrix))) * inNormal;
    fragColor = inNormal;
    
    gl_Position = projectMatrix * viewMatrix * worldPos;
}