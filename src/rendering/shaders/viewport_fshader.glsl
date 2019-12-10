#version 430

in  vec2 texturecoords;
out vec4 frag_color;

//uniform sampler2D texture_sampler;

void main() {
    frag_color = vec4(1.0, 0.0, 0.0, 1.0); //texture(texture_sampler, texturecoords);
}
