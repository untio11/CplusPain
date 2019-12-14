#include "Window.h"
#include "../../lib/glad/include/glad/glad.h"
#include "GLFW/glfw3.h"
#include <iostream>
#include "../rendering/Renderer.h"
#include "../rendering/Camera.h"

Camera* Window::cam = nullptr;

GLFWwindow* Window::init(const int width, const int height, const char* name, Camera* camera) {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    GLFWwindow* window = glfwCreateWindow(width, height, name, NULL, NULL);
    if (!window) {
        std::cerr << "Something went wrong with setting up the window\n";
    }

    glfwSetWindowAspectRatio(window, 16, 9);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetKeyCallback(window, keyCallback);
    glfwSetFramebufferSizeCallback(window, resizeCallback);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cerr << "Failed to initialize OpenGL context" << std::endl;
    }

    cam = camera;

    return window;
}

void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    } else if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
        cam->translate(glm::normalize(-cam->front));
    }
}

void Window::resizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    Renderer::resize(width, height);
}

void Window::cursorCallback(GLFWwindow* window, double x_pos, double y_pos) {
    cam->cameraCallback(window, x_pos, y_pos);
}