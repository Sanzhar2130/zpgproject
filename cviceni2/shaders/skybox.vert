#version 330 core
layout (location = 0) in vec3 vp;

out vec3 TexCoords;

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main() {
    TexCoords = vp;
    mat4 view = mat4(mat3(viewMatrix));
    vec4 pos = projectionMatrix * view * vec4(vp, 1.0);
    gl_Position = pos.xyww;
}