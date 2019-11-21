#ifndef CPLUSPAIN_WINDOW_H
#define CPLUSPAIN_WINDOW_H

class GLFWwindow; // From glfw3

namespace Window {
    void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    void resizeCallback(GLFWwindow* window, int width, int height);
    GLFWwindow* init(int width, int height, const char* name);
};


#endif //CPLUSPAIN_WINDOW_H
