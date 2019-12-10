#version 430

in  vec2 texturecoords;
out vec4 frag_color;

uniform sampler2D texture_sampler;

void main() {
    frag_color = texture(texture_sampler, texturecoords);
}
