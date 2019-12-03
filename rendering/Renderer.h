#ifndef CPLUSPAIN_RENDERER_H
#define CPLUSPAIN_RENDERER_H

class Renderer {
private:
    // In pixels for the viewport
    static int width;
    static int height;

    Renderer() = default; // Empty private constructor
    Renderer(Renderer const& copy); // No copy constructor
    Renderer& operator=(Renderer const& copy); // Assignment operator not supported

public:
    /**
     * Resize the viewport and update necessary stuff.
     * @param width The new width of the viewport.
     * @param height The new height of the viewport.
     */
    static void resize(int width, int height);
    static void init();
    static void render();

    static Renderer& getInstance() {
        static Renderer instance;
        return instance;
    }
};

#endif //CPLUSPAIN_RENDERER_H
