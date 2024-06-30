#version 330 core

out vec4 FragColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform bool useSolidColor;
uniform vec4 solidColor;
uniform vec3 lightColor;
uniform vec3 objectColor;

void main() {
    vec3 color;

    if (useSolidColor) {
        color = solidColor.rgb;
    } else {
        color = texture(texture1, TexCoord).rgb * objectColor;
    }

    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    vec3 result = ambient * color;
    FragColor = vec4(result, 1.0);
}