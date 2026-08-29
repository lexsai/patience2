#version 330 core
in vec2 vsTexCoord;
out vec4 FragColor;

uniform sampler2D uTexture;

void main() {
    FragColor = texture(uTexture, vsTexCoord) + vec4(0.05, 0.0, 0.0, 0.0);
}
