#version 330 core

in vec3 worldPosition;
in vec3 worldNormal;

out vec4 out_Color;

void main(void)
{
    vec3 lightPosition = vec3(10.0, 10.0, 10.0);

    vec3 lightToVector = lightPosition - worldPosition;

    float dotProduct = max(dot(normalize(lightToVector), normalize(worldNormal)), 0.0);

    vec4 objectColor = vec4(0.385, 0.647, 0.812, 1.0);
    vec4 diffuse = dotProduct * objectColor;

    vec4 ambient = vec4(0.1, 0.1, 0.1, 1.0);

    out_Color = ambient + diffuse;
}