#version 330 core

in vec3 fragNormal;
out vec4 fragColor;

void main()
{
    vec3 norm = normalize(fragNormal);
    fragColor = vec4(abs(norm), 1.0);
}