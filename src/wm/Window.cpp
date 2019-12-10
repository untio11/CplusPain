#include "Window.h"
#include "../../lib/glad/include/glad/glad.h"
#include "GLFW/glfw3.h"
#include <iostream>
#include "../rendering/Renderer.h"

GLFWwindow* Window::init(const int width, const int height, const char* name) {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    GLFWwindow* window = glfwCreateWindow(width, height, name, NULL, NULL);
    if (!window) {
        std::cerr << "Something went wrong with setting up the window\n";
    }

    glfwSetKeyCallback(window, keyCallback);
    glfwSetFramebufferSizeCallback(window, resizeCallback);
    Renderer::resize(width, height);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cerr << "Failed to initialize OpenGL context" << std::endl;
    }

    return window;
}

void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

void Window::resizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    Renderer::resize(width, height);
}