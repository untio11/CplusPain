cmake_minimum_required(VERSION 3.15)
project(CplusPain)

set(CMAKE_CXX_STANDARD 17)

add_executable(CplusPain main.cpp src/rendering/Renderer.cpp src/rendering/Renderer.h src/wm/Window.cpp src/wm/Window.h src/rendering/Debug.h src/rendering/Camera.cpp src/rendering/Camera.h src/rendering/ShaderProgram.cpp src/rendering/ShaderProgram.h lib/glad/include/glad/glad.h lib/glad/include/KHR/khrplatform.h lib/glad/src/glad.c)
target_link_libraries(CplusPain -lglfw3 -lopengl32 -lgdi32 -luser32)