#version 330 core

in vec3 worldPosition;
in vec3 worldNormal;
in vec2 uv;

uniform sampler2D textureUnitID;

out vec4 fragColor;

void main(void)
{
    // fragColor = vec4(uv, 1.0, 1.0);

    fragColor = texture(textureUnitID, uv);
}