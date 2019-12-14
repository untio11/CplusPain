#include "Window.h"
#include "../../lib/glad/include/glad/glad.h"
#include "GLFW/glfw3.h"
#include <iostream>
#include "../rendering/Renderer.h"
#include "../rendering/Camera.h"

Camera* Window::cam = nullptr;
std::set<int> Window::pressed_keys = std::set<int>();

GLFWwindow* Window::init(const int width, const int height, const char* name, Camera* camera) {
    if (!glfwInit())
        std::cout << "Something went wrong with setting up GLFW\n";
    glfwSetErrorCallback(errorCallback);

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
    glfwSetCursorPosCallback(window, cursorCallback);
    glfwSetScrollCallback(window, scrollCallback);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cerr << "Failed to initialize OpenGL context" << std::endl;
    }

    cam = camera;

    return window;
}

void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        pressed_keys.insert(key);
    } else if (action == GLFW_RELEASE) {
        pressed_keys.erase(key);
    }
}

void Window::resizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    Renderer::resize(width, height);
}

void Window::cursorCallback(GLFWwindow* window, double x_pos, double y_pos) {
    cam->cameraCallback(window, x_pos, y_pos);
}

void Window::scrollCallback(GLFWwindow *window, double xoffset, double y_offset) {
    cam->zoom(y_offset * 0.1);
}

void Window::errorCallback(int error, const char* description) {
    fprintf_s(stderr, "Error: %s\n", description);
}

std::set<int>& Window::getPressedKeys() {
    return pressed_keys;
}
