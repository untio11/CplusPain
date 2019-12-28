#include "Renderer.h"
#include "ShaderProgram.h"
#include "Camera.h"
#include <iostream>
#include <cmath>
#include <glm/gtc/type_ptr.hpp>
#include <glfw/glfw3.h>

// Initialize static member variables.
int Renderer::width = 0;
int Renderer::height = 0;
ShaderProgram* Renderer::viewport_shader = nullptr;
ShaderProgram* Renderer::raytrace_shader = nullptr;
unsigned int Renderer::viewport_vao = 0;
unsigned int Renderer::viewport_index_vbo = 0;
unsigned int Renderer::raytrace_image = 0;
int Renderer::raytrace_work_group_dim[] = {0, 0, 0};
Camera* Renderer::camera = nullptr;
int Renderer::AA_level = 1;

void Renderer::resize(int _width, int _height) {
    width = _width;
    height = _height;
    Renderer::setupTexture();
}

void Renderer::init() {
    setupViewportQuad();
    setupViewportProgram();
    setupTexture();
    setupRaytraceProgram();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, raytrace_image);
    glProgramUniform1i(raytrace_shader->getID(), 4, AA_level);
}

void Renderer::render() {
    glClearColor(0.5, 0.5, 0.5, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    for (int i = 0; i < AA_level; ++i) {
        glProgramUniform1i(raytrace_shader->getID(), 3, i);
        rayTrace();
        glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
    }
    renderViewport();
    glClearTexImage(raytrace_image, 0, GL_RGBA, GL_FLOAT, NULL);
}

void Renderer::rayTrace() {
    loadCameraData(*camera);
    raytrace_shader->use();
    glDispatchCompute(
            ceil(width  * scaling / raytrace_work_group_dim[0]),
            ceil(height * scaling / raytrace_work_group_dim[1]),
            1
    );
}

void Renderer::renderViewport() {
    viewport_shader->use();

    glBindVertexArray(viewport_vao);
    glEnableVertexAttribArray(0); // Enable vertex position data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, viewport_index_vbo);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

    // Unbind everything.
    glDisableVertexAttribArray(0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
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
    viewport_shader = new ShaderProgram();
    viewport_shader->addFile("../src/rendering/shaders/viewport_vshader.glsl", GL_VERTEX_SHADER);
    viewport_shader->addFile("../src/rendering/shaders/viewport_fshader.glsl", GL_FRAGMENT_SHADER);
    viewport_shader->createProgram();
}

void Renderer::setupTexture() {
    glGenTextures(1, &raytrace_image);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, raytrace_image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width * scaling, height * scaling, 0, GL_RGBA, GL_FLOAT, nullptr);
    glBindImageTexture(0, raytrace_image, 0, false, 0, GL_READ_WRITE, GL_RGBA32F);
}

Renderer::~Renderer() {
    delete(viewport_shader);
    delete(raytrace_shader);
}

void Renderer::setupRaytraceProgram() {
    raytrace_shader = new ShaderProgram();
    raytrace_shader->addFile("../src/rendering/shaders/raytracer.glsl", GL_COMPUTE_SHADER);
    raytrace_shader->createProgram();
    glGetProgramiv(raytrace_shader->getID(), GL_COMPUTE_WORK_GROUP_SIZE, raytrace_work_group_dim);
    std::cout << "[Info] Work group dimensions: " << raytrace_work_group_dim[0] << ", "
        << raytrace_work_group_dim[1] << ", " << raytrace_work_group_dim[2] << std::endl;
    int work_grp_cnt[3];

    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &work_grp_cnt[0]);
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1, &work_grp_cnt[1]);
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2, &work_grp_cnt[2]);

    printf("[Info] Max global (total) work group size x:%i y:%i z:%i\n",
           work_grp_cnt[0], work_grp_cnt[1], work_grp_cnt[2]);

    int work_grp_size[3];

    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &work_grp_size[0]);
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1, &work_grp_size[1]);
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 2, &work_grp_size[2]);

    printf("[Info] Max local (in one shader) work group sizes x:%i y:%i z:%i\n",
           work_grp_size[0], work_grp_size[1], work_grp_size[2]);

    int work_grp_inv;
    glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &work_grp_inv);
    printf("[Info] Max local work group invocations %i\n", work_grp_inv);
}

void Renderer::loadCameraData(Camera& _camera) { // Might keep track of last position to copy less.
    glProgramUniform3fv(raytrace_shader->getID(), 0, 1, glm::value_ptr(_camera.getPosition()));
    glProgramUniform1f(raytrace_shader->getID(), 1, _camera.getCameraDistance());
    glProgramUniformMatrix3fv(raytrace_shader->getID(), 2, 1, false, glm::value_ptr(_camera.getRotationMatrix()));
}

void Renderer::setCamera(Camera* _camera) {
    camera = _camera;
}

void Renderer::changeAA(int delta) {
    AA_level = __max(1, AA_level + delta);
    glProgramUniform1i(raytrace_shader->getID(), 4, AA_level);
    std::cout << "[Info] AA_level = " << AA_level << std::endl;
}

