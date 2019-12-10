#include "Renderer.h"
#include "ShaderProgram.h"
#include <iostream>

// Initialize static member variables.
int Renderer::width = 0;
int Renderer::height = 0;

void Renderer::resize(int _width, int _height) {
    width = _width;
    height = _height;
}

void Renderer::init() {

}

void Renderer::setupViewportQuad() {
    glGenVertexArrays(1, &viewport_vao);
    glBindVertexArray(viewport_vao);

    unsigned int coord_vbo;
    glGenBuffers(1, &coord_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, coord_vbo);
    glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(float), viewport, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, false, 0, nullptr);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &viewport_index_vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, viewport_index_vbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), viewport_indices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
}

void Renderer::setupViewportProgram() {
    ShaderProgram program = ShaderProgram();
    program.addFile("../src/rendering/shaders/viewport_vshader.glsl", GL_VERTEX_SHADER);
    program.addFile("../src/rendering/shaders/viewport_fshader.glsl", GL_FRAGMENT_SHADER);
    viewport_shader = program.createProgram();
}

void Renderer::setupTexture() {
    glGenTextures(1, &raytrace_image);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, raytrace_image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
    glBindImageTexture(0, raytrace_image, 0, false, 0, GL_READ_WRITE, GL_RGBA32F);
}

void Renderer::render() {
    //rayTrace();
    renderViewport();
}

void Renderer::rayTrace() {

}

void Renderer::renderViewport() {
    std::cerr << glGetError() << " : ";
    glUseProgram(viewport_shader);
    std::cerr << glGetError() << std::endl;
    //glActiveTexture(GL_TEXTURE0);
    //glBindTexture(GL_TEXTURE_2D, raytrace_image);

    glBindVertexArray(viewport_vao);
    glEnableVertexAttribArray(0); // Enable vertex position data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, viewport_index_vbo);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

    // Unbind everything.
    std::cerr << glGetError() << " : ";
    glUseProgram(0);
    glDisableVertexAttribArray(0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    //glActiveTexture(0);
    std::cerr << glGetError() << std::endl;
}

Renderer::Renderer() {
    setupViewportQuad();
    setupViewportProgram();
    //setupTexture();
}

