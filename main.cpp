#include <iostream>
#define TINYOBJLOADER_IMPLEMENTATION
//#include <tiny_obj_loader.h>

#include <GLFW/glfw3.h>

static void errorCallback(int error, const char* description) {
    fprintf_s(stderr, "Error: %s\n", description);
}

static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

static void resizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

int main() {
    if (!glfwInit())
        std::cout << "Something went wrong with setting up GLFW\n";
    glfwSetErrorCallback(errorCallback);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);


    GLFWwindow* window = glfwCreateWindow(720, 480, "Testing Title", NULL, NULL);
    glfwSetKeyCallback(window, keyCallback);
    glfwSetFramebufferSizeCallback(window, resizeCallback);

    if (!window)
        std::cout << "Something went wrong with setting up the window\n";
    glfwMakeContextCurrent(window);

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    resizeCallback(window, width, height); // Possibly not necessary.
    glfwSwapInterval(1);
    float red = 0;
    float green = 0;
    float blue = 0;
    bool up = true;

    while (!glfwWindowShouldClose(window)) {
        glClearColor(red, green, blue, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        glfwPollEvents();
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