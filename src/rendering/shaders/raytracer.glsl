#version 430
layout(local_size_x = 4, local_size_y = 4, local_size_z = 1) in;
layout(rgba32f, binding = 0) uniform restrict coherent image2D img_output;

layout(location = 0) uniform vec3  camera;
layout(location = 1) uniform float fov;
layout(location = 2) uniform mat3  transform;
layout(location = 3) uniform int seed;
layout(location = 4) uniform int AA_level;

// A single iteration of Bob Jenkins' One-At-A-Time hashing algorithm.
uint hash( uint x ) {
    x += ( x << 10u );
    x ^= ( x >>  6u );
    x += ( x <<  3u );
    x ^= ( x >> 11u );
    x += ( x << 15u );
    return x;
}

// Compound versions of the hashing algorithm I whipped together.
uint hash( uvec2 v ) { return hash( v.x ^ hash(v.y)                         ); }
uint hash( uvec3 v ) { return hash( v.x ^ hash(v.y) ^ hash(v.z)             ); }
uint hash( uvec4 v ) { return hash( v.x ^ hash(v.y) ^ hash(v.z) ^ hash(v.w) ); }

// Construct a float with half-open range [0:1] using low 23 bits.
// All zeroes yields 0.0, all ones yields the next smallest representable value below 1.0.
float floatConstruct( uint m ) {
    const uint ieeeMantissa = 0x007FFFFFu; // binary32 mantissa bitmask
    const uint ieeeOne      = 0x3F800000u; // 1.0 in IEEE binary32

    m &= ieeeMantissa;                     // Keep only mantissa bits (fractional part)
    m |= ieeeOne;                          // Add fractional part to 1.0

    float  f = uintBitsToFloat( m );       // Range [1:2]
    return f - 1.0;                        // Range [0:1]
}

// Pseudo-random value in half-open range [0:1].
float random( float x ) { return floatConstruct(hash(floatBitsToUint(x))); }
float random( vec2  v ) { return floatConstruct(hash(floatBitsToUint(v))); }
float random( vec3  v ) { return floatConstruct(hash(floatBitsToUint(v))); }
float random( vec4  v ) { return floatConstruct(hash(floatBitsToUint(v))); }

vec3 discriminant(vec3 ray, vec3 source, vec3 target, float sphere_radius) {
    vec3 omc = source - target;
    float b = dot(omc, ray);
    float c = dot(omc, omc) - sphere_radius * sphere_radius;
    return vec3(b, c, (b * b - c));
}

vec3 getRay() {
    vec2 jitter = (AA_level > 1) ? vec2(0.1 + 0.8 * random(vec2(seed, gl_GlobalInvocationID.x)), 0.1 + 0.8 * random(vec2(seed, gl_GlobalInvocationID.y))) : vec2(0.5);
    vec2 pixel_coords = gl_GlobalInvocationID.xy + jitter;
    vec2 dimensions = imageSize(img_output);
    // Map pixel coordinates to normalized space: [-1,1]^2 (sorta, taking care of aspect ratio)
    float x = (float(pixel_coords.x * 2.0 - dimensions.x) / (dimensions.x)) * (16.0/9.0) + camera.x;
    float y = (float(pixel_coords.y * 2.0 - dimensions.y) / dimensions.y) + camera.y;
    float z = camera.z + fov;

    return normalize((vec3(x, y, z) - camera) * transform);
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
        return phong(intersection, planenormal, vec3(0.0, 4.0, 0.0), col, 0.01);
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

    vec4 color = trace() / float(AA_level);
    vec4 base_color = imageLoad(img_output, pixel_coords);
    imageStore(img_output, pixel_coords, base_color + color);
}
