#version 330 core

out vec4 FragColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform bool useSolidColor;
uniform vec4 solidColor;

void main() {
    if (useSolidColor) {
        FragColor = solidColor;
    } else {
        FragColor = texture(texture1, TexCoord);
    }
}