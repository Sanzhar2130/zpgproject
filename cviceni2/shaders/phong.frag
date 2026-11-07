#version 330 core

in vec3 worldPosition;
in vec3 worldNormal;

uniform vec3 lightPosition; 
uniform vec3 cameraPosition;

uniform mat4 viewMatrix;

out vec4 fragColor;

void main(void)
{
    vec3 N = normalize(worldNormal);
    vec3 L = normalize(lightPosition - worldPosition);
    vec3 V = normalize(cameraPosition - worldPosition);
    vec3 R = reflect(-L, N);

    vec4 ambient = vec4(0.1, 0.1, 0.1, 1.0);

    float diff = max(dot(N, L), 0.0);
    vec4 objectColor = vec4(0.385, 0.647, 0.812, 1.0);
    vec4 diffuse = diff * objectColor;

    float shininess = 32.0;
    float spec = pow(max(dot(R, V), 0.0), shininess);
    vec4 specular = spec * vec4(1.0, 1.0, 1.0, 1.0);

    if (diff <= 0.0) {
        specular = vec4(0.0);
    }

    fragColor = ambient + diffuse + specular;
}