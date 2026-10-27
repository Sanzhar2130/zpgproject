#version 330 core

in vec3 worldPosition;
in vec3 worldNormal;

uniform mat4 viewMatrix;

out vec4 out_Color;

void main(void)
{
    vec3 lightPosition = vec3(0.0, 0.0, 0.0);

    vec3 lightVector = normalize(lightPosition - worldPosition);
    vec3 normal = normalize(worldNormal);

    vec3 cameraPosition = inverse(viewMatrix)[3].xyz;
    vec3 viewDir = normalize(cameraPosition - worldPosition);

    vec4 ambient = vec4(0.1, 0.1, 0.1, 1.0);

    float dotProduct = max(dot(lightVector, normal), 0.0);
    vec4 diffuse = dotProduct * vec4(0.385, 0.647, 0.812, 1.0);

    vec3 reflectDir = reflect(-lightVector, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    vec4 specular = spec * vec4(1.0, 1.0, 1.0, 1.0);

    out_Color = ambient + diffuse + specular;
}