#version 430
layout(local_size_x = 32, local_size_y = 32) in;
layout(rgba32f, binding = 0) uniform image2D img_output;

void main() {
    // get index in global work group i.e x,y position
    ivec2 pixel_coords = ivec2(gl_GlobalInvocationID.xy);
    vec4 color = vec4(gl_LocalInvocationID, 1.0);
    //
    // interesting stuff happens here later
    //

    // output to a specific pixel in the image
    imageStore(img_output, pixel_coords, color);
}
