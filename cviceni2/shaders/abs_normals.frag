#version 330 core

in vec3 fragNormal;
in vec3 fragWorldPos;

out vec4 outColor;

void main()
{
    outColor = vec4(abs(normalize(fragNormal)), 1.0);
}