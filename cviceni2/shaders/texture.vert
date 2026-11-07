#version 330 core

layout (location = 0) in vec3 vp;
layout (location = 1) in vec3 vn;
layout (location = 2) in vec2 uvAttr;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectMatrix;

out vec3 worldPosition;
out vec3 worldNormal;
out vec2 uv;

void main(void)
{
    vec4 wp = modelMatrix * vec4(vp, 1.0);
    worldPosition = wp.xyz / wp.w;

    mat3 normalMatrix = transpose(inverse(mat3(modelMatrix)));
    worldNormal = normalMatrix * vn;

    uv = uvAttr;

    gl_Position = projectMatrix * viewMatrix * wp;
}