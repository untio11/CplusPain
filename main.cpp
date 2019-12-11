#include <iostream>
#define TINYOBJLOADER_IMPLEMENTATION
//#include <tiny_obj_loader.h>
#include "lib/glad/include/glad/glad.h"
#include <GLFW/glfw3.h>
#include "src/wm/Window.h"
#include "src/rendering/Renderer.h"

static void errorCallback(int error, const char* description) {
    fprintf_s(stderr, "Error: %s\n", description);
}

GLFWwindow* initGLFW() {
    if (!glfwInit())
        std::cout << "Something went wrong with setting up GLFW\n";
    glfwSetErrorCallback(errorCallback);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    return Window::init(720, 480, "Test");
}

int main() {
    GLFWwindow* window = initGLFW();
    Renderer::init();
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    Renderer::resize(width, height);
    double previous = glfwGetTime();
    double lag = 0.0;
    double current;
    double elapsed;
    double accumulated = 0.0;
    unsigned int iterations = 0;
    const double timestep = 6.0 / 1000; // In ms.
    const unsigned int frames = 300;
    std::cerr << "Setup time: " << previous << std::endl;

    while (!glfwWindowShouldClose(window)) {
        current = glfwGetTime();
        elapsed = current - previous;
        accumulated += elapsed;
        previous = current;
        lag += elapsed;

        if (iterations == frames) {
            std::cout << "Avg Frametime over last "<< frames << " frames: " << accumulated * 1000 / frames << "ms.\n";
            accumulated = 0;
            iterations = 0;
        }

        glfwPollEvents();

        while (lag >= timestep) {
            // Update simulations and stuff.
            lag -= timestep;
        }

        Renderer::render();
        glfwSwapBuffers(window);
        ++iterations;
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}