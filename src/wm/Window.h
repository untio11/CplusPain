#ifndef CPLUSPAIN_WINDOW_H
#define CPLUSPAIN_WINDOW_H

#include <set>

class GLFWwindow; // From glfw3
class Camera;

class Window {
private:
    static Camera* cam;
    static std::set<int> pressed_keys;
public:
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void resizeCallback(GLFWwindow* window, int width, int height);
    static void cursorCallback(GLFWwindow* window, double x_pos, double y_pos);
    static void scrollCallback(GLFWwindow* window, double x_offset, double y_offset);
    static GLFWwindow* init(int width, int height, const char* name, Camera* camera);
    static std::set<int>& getPressedKeys();
    static void errorCallback(int error, const char *description);
};


#endif //CPLUSPAIN_WINDOW_H
