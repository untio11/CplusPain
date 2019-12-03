#include "Window.h"
#include <iostream>
#include "GLFW/glfw3.h"
#include "../rendering/Renderer.h"

GLFWwindow* Window::init(const int width, const int height, const char* name) {
    GLFWwindow* window = glfwCreateWindow(width, height, name, NULL, NULL);
    if (!window) {
        std::cout << "Something went wrong with setting up the window\n";
    }

    glfwSetKeyCallback(window, keyCallback);
    glfwSetFramebufferSizeCallback(window, resizeCallback);
    Renderer::resize(width, height);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
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