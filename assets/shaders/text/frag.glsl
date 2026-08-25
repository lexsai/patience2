#version 330 core
in vec2 vsTexCoord;
out vec4 FragColor;

uniform sampler2D uTexture;

void main() {
    FragColor = vec4(1.0, 1.0, 1.0, texture(uTexture, vsTexCoord).r);
}
