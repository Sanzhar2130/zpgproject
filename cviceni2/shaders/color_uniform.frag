#version 330 core

in vec3 fragNormal;
in vec3 fragWorldPos;

uniform vec3 uColor = vec3(1.0, 1.0, 1.0);

out vec4 outColor;

void main()
{
    vec3 lightDir = normalize(vec3(0.5, 1.0, 0.8));
    vec3 norm = normalize(fragNormal);
    float diff = max(dot(norm, lightDir), 0.25); 

    outColor = vec4(uColor * diff, 1.0);
}