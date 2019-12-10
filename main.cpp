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
    float red = 0.5f;
    float green = 0.5f;
    float blue = 0.5f;

    while (!glfwWindowShouldClose(window)) {
        glClearColor(red, green, blue, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        glfwPollEvents();

        Renderer::render();
        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}