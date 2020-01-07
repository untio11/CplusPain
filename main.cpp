#include <iostream>
#include <GLFW/glfw3.h>
#include "src/wm/Window.h"
#include "src/rendering/Renderer.h"
#include "src/rendering/Camera.h"
#include "src/wm/KeyHandler.h"

#include "src/scene/Prop.h"

void test() {
    Prop test_prop = Prop();
    test_prop.loadObj("../res/CornellBox-Original.obj");
}

int main() {
    Camera cam;
    GLFWwindow* window = Window::init(720, 480, "Test", &cam);
    Renderer::init();
    Renderer::setCamera(&cam);

    KeyHandler key_handler = KeyHandler(&cam, window);

    int width, height;
    glfwGetWindowSize(window, &width, &height);
    Renderer::resize(width, height);

	test();
    double previous = glfwGetTime();
    std::cout << "[Info] Setup time: " << previous << std::endl;

	double lag = 0.0;
	double current;
	double elapsed;
	double accumulated = 0.0;
	unsigned int iterations = 0;
	const double timestep = 10.0 / 1000.0; // In ms.
	const unsigned int frames = 300;


    while (!glfwWindowShouldClose(window)) {
        current = glfwGetTime();
        elapsed = current - previous;
        accumulated += elapsed;
        previous = current;
        lag += elapsed;

        if (iterations == frames) {
            std::cout << "[Info] Avg Frametime over last "<< frames << " frames: " << accumulated * 1000 / frames << "ms.\n";
            accumulated = 0;
            iterations = 0;
        }

        glfwPollEvents();

        while (lag >= timestep) {
            // Update simulations and stuff.
            key_handler.handleKeys(Window::getPressedKeys(), lag/timestep);
            lag -= timestep;
        }
        Renderer::render();
        glfwSwapBuffers(window);
        ++iterations;
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}