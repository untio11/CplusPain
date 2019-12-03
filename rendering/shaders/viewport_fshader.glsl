#version 430

in  vec2 texturecoords;
out vec4 color;

uniform sampler2D texture_sampler;

void main() {
    color = texture(texture_sampler, texturecoords);
}
