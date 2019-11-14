cmake_minimum_required(VERSION 3.15)
project(CplusPain)

set(CMAKE_CXX_STANDARD 17)

add_executable(CplusPain main.cpp)
target_link_libraries(CplusPain -lglfw3 -lopengl32 -lgdi32 -luser32)