#version 430
layout(local_size_x = 32, local_size_y = 32) in;
layout(rgba32f, binding = 0) uniform image2D img_output;

void main() {
    // get index in global work group i.e x,y position
    ivec2 pixel_coords = ivec2(gl_GlobalInvocationID.xy);
    vec2 c = gl_GlobalInvocationID.xy;
    vec4 color = vec4(c / imageSize(img_output).xy, 0.0, 1.0);

    imageStore(img_output, pixel_coords, color);
}
