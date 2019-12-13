#version 430
layout(local_size_x = 16, local_size_y = 16) in;
layout(rgba32f, binding = 0) uniform image2D img_output;

layout(location = 0) uniform vec3  camera;
layout(location = 1) uniform float fov;
layout(location = 2) uniform mat3  transform;

vec3 discriminant(vec3 ray, vec3 source, vec3 target, float sphere_radius) {
    vec3 omc = source - target;
    float b = dot(omc, ray);
    float c = dot(omc, omc) - sphere_radius * sphere_radius;
    return vec3(b, c, (b * b - c));
}

vec3 getRay() {
    vec2 pixel_coords = gl_GlobalInvocationID.xy;
    vec2 dimensions = imageSize(img_output);
    // Map pixel coordinates to normalized space: [-1,1]^2 (sorta, taking care of aspect ratio)
    float x = (float(pixel_coords.x * 2.0 - dimensions.x) / (dimensions.x)) * (16.0/9.0) + camera.x;
    float y = (float(pixel_coords.y * 2.0 - dimensions.y) / dimensions.y) + camera.y;
    float z = camera.z + fov;

    return normalize(vec3(x, y, z) - camera) * transform;
}

vec4 phong(vec3 point, vec3 normal, vec3 light_source, vec4 sphere_color, float shininess) {
    vec4 result = 0.0 * sphere_color; // ambient light initial color

    vec3 L = (light_source - point); // Direction from point to light source
    float angle = max(dot(normal, L) / (length(normal)*length(L)), 0.0); // Angle between normal and light source

    // Diffuse contribution
    result += angle * sphere_color;

    vec3 E = normalize(point - camera); // viewdirection
    vec3 halfway = normalize(L + E);
    float spec = pow(max(dot(normal, halfway), 0.0), shininess);
    result += spec * sphere_color;
    return result;
}

vec4 planeIntersect(vec3 ray, float closest) {
    vec3 planepoint = vec3(0.0, -6.0, 0.0);
    vec3 planenormal = vec3(0.0, 1.0, 0.0);

    float ndotu = dot(ray, planenormal); // 0 if ray and plane are parallel.

    float d = (dot((planepoint - camera), planenormal) / ndotu ); // find intersection with bottom plane
    vec3 intersection = d * ray + camera;

    if (d >= 0.0  && d <= closest) {
        vec4 col = ((abs(int(intersection.x - 0.5) % 2) < 0.001) && !(abs(int(intersection.z - 0.5) % 2) < 0.001)) ? vec4(0.3, 0.3, 0.3, 1.0) : vec4(0.6, 0.6, 0.6, 1.0);
        return phong(intersection, planenormal, vec3(0.0, 4.0, 0.0), col, 0.1);
    }
    return vec4(0.53, 0.81, 0.98, 1.0);
}

vec4 trace() {
    vec4 color = vec4(0.0, 0.0, 0.0, 1.0); // Base color
    vec3 direction = getRay();
    float smallest = 1.0 / 0.0; // infinity (?)
    vec3 sphere_center = vec3(0.0, 1.0, 4.0);
    float sphere_radius = 0.5;

    vec3 disc = discriminant(direction, camera, sphere_center, sphere_radius);
    float distance = min(-disc.x + sqrt(disc.z), -disc.x - sqrt(disc.z));

    if (disc.z < 0 || distance <= 0) return planeIntersect(direction, smallest); // no intersection

    smallest = distance < smallest ? distance : smallest;

    if (distance >= 0 && smallest == distance) { // If the current sphere is the closest and it hits, color it
        vec3 intersection = distance * direction + camera;
        vec3 sphere_normal = (intersection - sphere_center) / sphere_radius;

        color += phong(intersection, sphere_normal, vec3(0.0, 4.0, 0.0), vec4(0.75, 0.0, 0.75, 1.0), 1.0);

    }

    return color;
}

void main() {
    // get index in global work group i.e x,y position
    ivec2 pixel_coords = ivec2(gl_GlobalInvocationID.xy);

    vec4 color = trace();

    imageStore(img_output, pixel_coords, color);
}
