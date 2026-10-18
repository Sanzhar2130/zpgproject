#version 330 core

in vec3 fragColor;
in vec3 fragNormal;
flat in int isLeaf;

out vec4 outColor;

void main()
{
    vec3 norm = normalize(fragNormal);

    if (isLeaf == 1) {
        float shade = 0.3 + 0.7 * abs(norm.y);
        outColor = vec4(shade * fragColor, 1.0);
    } else {
        float trunkShade = 0.55 + 0.45 * abs(norm.y);
        outColor = vec4(trunkShade * fragColor, 1.0);
    }
}