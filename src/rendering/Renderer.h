#ifndef CPLUSPAIN_RENDERER_H
#define CPLUSPAIN_RENDERER_H

class ShaderProgram;

class Renderer {
private:
    // In pixels for the viewport
    static int width;
    static int height;
    void setupViewportQuad();
    void setupViewportProgram();
    // Shaders
    ShaderProgram* viewport_shader;
    ShaderProgram* raytrace_shader;
    // Viewport data
    unsigned int viewport_vao;
    unsigned int viewport_index_vbo;
    // Texture that the raytracer will render to.
    unsigned int raytrace_image;
    // Raytracer work group dimensions
    int raytrace_work_group_dim[3];
    // Scaling parameter for image quality.
    const float scaling = 1.0f;
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
    void renderViewport();
    void rayTrace();
    void setupTexture();
    void setupRaytraceProgram();

public:
    /**
     * Resize the viewport and update necessary stuff.
     * @param width The new width of the viewport.
     * @param height The new height of the viewport.
     */
    static void resize(int width, int height);

    void init();
    void render();
    Renderer();
    ~Renderer();
};

#endif //CPLUSPAIN_RENDERER_H
