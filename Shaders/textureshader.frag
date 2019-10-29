#version 330 core

in vec2 UV;
uniform sampler2D textureSampler;
uniform vec3 objectColor = vec3(1.0, 1.0, 1.0);
out vec3 fragColor;

void main() {
    fragColor = texture(textureSampler, UV).rgb * objectColor;
}
