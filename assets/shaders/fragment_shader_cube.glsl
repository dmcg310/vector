#version 330 core

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform sampler2D texture1;
uniform bool useSolidColor;
uniform vec4 solidColor;

void main() {
    vec3 color;
    if (useSolidColor) {
        color = solidColor.rgb;
    } else {
        color = texture(texture1, TexCoord).rgb * objectColor;
    }

    // Ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    // Diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 result = (ambient + diffuse) * color;
    FragColor = vec4(result, 1.0);
}