#version 430

layout(location = 0) in vec4 position_in;
out vec2 texturecoords;

void main() {
    gl_Position = position_in;
    // We know we get a quad on [-1,1]^2, so simply remap to [0,1]^2.
    texturecoords = position_in.xy * 0.5 + vec2(0.5, 0.5);
}