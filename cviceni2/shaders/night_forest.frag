#version 330 core

in vec3 worldPosition;
in vec3 worldNormal;

uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform vec3 cameraPosition;

uniform float constantAttenuation;
uniform float linearAttenuation;
uniform float quadraticAttenuation;

out vec4 out_Color;

void main(void)
{
    vec3 ambient = vec3(0.05, 0.05, 0.08) * objectColor;

    vec3 lightDir = lightPosition - worldPosition;
    float distance = length(lightDir);
    lightDir = normalize(lightDir);

    vec3 normal = normalize(worldNormal);

    float attenuation = 1.0 / (constantAttenuation + linearAttenuation * distance + quadraticAttenuation * (distance * distance));

    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * lightColor * objectColor;

    vec3 viewDir = normalize(cameraPosition - worldPosition);
    vec3 halfVector = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfVector), 0.0), 16.0);
    vec3 specular = spec * lightColor * vec3(0.3);

    if (diff <= 0.0) {
        specular = vec3(0.0);
    }

    vec3 result = ambient + attenuation * (diffuse + specular);

    out_Color = vec4(result, 1.0);
}