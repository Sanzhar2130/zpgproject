#version 330 core

in vec3 fragColor;
in vec3 fragNormal;

out vec4 outColor;

void main()
{
    vec3 finalColor = fragColor;

    if (fragColor.r > 0.7 && fragColor.g > 0.7 && fragColor.b < 0.3) {
        finalColor = vec3(5.0 / 255.0, 195.0 / 255.0, 222.0 / 255.0);
    }
    else if (fragColor.g > 0.8 && fragColor.r < 0.2 && fragColor.b < 0.2) {
        finalColor = vec3(0.08, 0.08, 0.08);
    }
    else if (fragColor.b > 0.8 && fragColor.r < 0.2 && fragColor.g < 0.2) {
        finalColor = vec3(0.08, 0.08, 0.08);
    }
    else if (fragColor.r > 0.8 && fragColor.g < 0.2 && fragColor.b < 0.2) {
        finalColor = vec3(0.95, 0.05, 0.05);
    }

    outColor = vec4(finalColor, 1.0);
}