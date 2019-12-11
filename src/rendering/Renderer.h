#ifndef CPLUSPAIN_RENDERER_H
#define CPLUSPAIN_RENDERER_H

class ShaderProgram;

class Renderer {
private:
    // In pixels for the viewport
    static int width;
    static int height;
    // Shaders
    static ShaderProgram* viewport_shader;
    static ShaderProgram* raytrace_shader;
    // Viewport data
    static unsigned int viewport_vao;
    static unsigned int viewport_index_vbo;
    // Texture that the raytracer will render to.
    static unsigned int raytrace_image;
    // Raytracer work group dimensions
    static int raytrace_work_group_dim[3];
    // Scaling parameter for image quality.
    static constexpr float scaling = 3.0f;
    // Coordinates and indices for the viewport.
    static constexpr float viewport[16] {
            -1.0f,  1.0f, 0.0f, 1.0f, // 1/6 -> ID:0
            -1.0f, -1.0f, 0.0f, 1.0f, // 2   -> ID:1
             1.0f, -1.0f, 0.0f, 1.0f, // 3/4 -> ID:2
             1.0f,  1.0f, 0.0f, 1.0f, // 5   -> ID:3
    };
    static constexpr unsigned int viewport_indices[6] {
            0, 1, 2,
            2, 3, 0
    };
    static void setupViewportQuad();
    static void setupViewportProgram();
    static void renderViewport();
    static void rayTrace();
    static void setupTexture();
    static void setupRaytraceProgram();

public:
    /**
     * Resize the viewport and update necessary stuff.
     * @param width The new width of the viewport.
     * @param height The new height of the viewport.
     */
    static void resize(int width, int height);
    static void init();
    static void render();
    ~Renderer();
};

#endif //CPLUSPAIN_RENDERER_H
