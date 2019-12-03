#include <iostream>
#define TINYOBJLOADER_IMPLEMENTATION
//#include <tiny_obj_loader.h>
#include <GLFW/glfw3.h>
#include "wm/Window.h"
#include "rendering/Renderer.h"

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

    float red = 0;
    float green = 0;
    float blue = 0;
    bool up = true;

    while (!glfwWindowShouldClose(window)) {
        glClearColor(red, green, blue, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        glfwPollEvents();

        Renderer::render();
        glfwSwapBuffers(window);

        if (up && red < 1.0) {
            red += 1.0/256;
        } else if (up && green < 1.0) {
            green += 1.0/256;
        } else if (up && blue < 1.0) {
            blue += 1.0/256;
        } else if (!up && red >= 0.0) {
            red -= 1.0/256;
        } else if (!up && green >= 0.0) {
            green -= 1.0/256;
        } else if (!up && blue >= 0.0) {
            blue -= 1.0/256;
        } else {
            up = !up;
        }
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}