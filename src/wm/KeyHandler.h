#ifndef CPLUSPAIN_KEYHANDLER_H
#define CPLUSPAIN_KEYHANDLER_H

#include <set>
#include "../rendering/Camera.h"
#include <GLFW/glfw3.h>
#include <glm/gtx/string_cast.hpp>
#include "../rendering/Renderer.h"

class KeyHandler {
private:
    Camera* camera;
    GLFWwindow* window;
public:
    KeyHandler(Camera* _camera, GLFWwindow* _window) {
        camera = _camera;
        window = _window;
    };

    void handleKeys(std::set<int> &pressed_keys, double delta_t) {
        for (auto key : pressed_keys) {
            switch (key) {
                case GLFW_KEY_ESCAPE:
                    glfwSetWindowShouldClose(window, true);
                    break;
                case GLFW_KEY_W:
                    camera->translate(camera->front, delta_t);
                    break;
                case GLFW_KEY_S:
                    camera->translate(-camera->front, delta_t);
                    break;
                case GLFW_KEY_A:
                    camera->translate(-camera->right, delta_t);
                    break;
                case GLFW_KEY_D:
                    camera->translate(camera->right, delta_t);
                    break;
                case GLFW_KEY_MINUS:
                    Renderer::changeAA(-1);
                    pressed_keys.erase(key);
                    break;
                case GLFW_KEY_EQUAL:
                    Renderer::changeAA(1);
                    pressed_keys.erase(key);
                    break;
                default:
                    break;
            }
        }
    };
};

#endif //CPLUSPAIN_KEYHANDLER_H
