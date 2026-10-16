#version 330 core

in vec3 fragColor;
in vec3 fragNormal;

out vec4 outColor;

void main()
{
    vec3 finalColor = fragColor;

    // 1. Логотип кафедры KI и полосы (в модели имеют желтый цвет: R > 0.7, G > 0.7, B < 0.3)
    // Окрашиваем в официальный Pantone 311 C (RGB: 5, 195, 222)
    if (fragColor.r > 0.7 && fragColor.g > 0.7 && fragColor.b < 0.3) {
        finalColor = vec3(5.0 / 255.0, 195.0 / 255.0, 222.0 / 255.0);
    }
    // 2. Минутная стрелка (в модели зелёная) -> делаем контрастной черной
    else if (fragColor.g > 0.8 && fragColor.r < 0.2 && fragColor.b < 0.2) {
        finalColor = vec3(0.08, 0.08, 0.08);
    }
    // 3. Часовая стрелка (в модели синяя) -> делаем контрастной черной
    else if (fragColor.b > 0.8 && fragColor.r < 0.2 && fragColor.g < 0.2) {
        finalColor = vec3(0.08, 0.08, 0.08);
    }
    // 4. Секундная стрелка (в модели красная) -> чистый красный цвет
    else if (fragColor.r > 0.8 && fragColor.g < 0.2 && fragColor.b < 0.2) {
        finalColor = vec3(0.95, 0.05, 0.05);
    }

    outColor = vec4(finalColor, 1.0);
}