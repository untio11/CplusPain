#include "Renderer.h"
#include "ShaderProgram.h"
#include <iostream>
#include <math.h>

// Initialize static member variables.
int Renderer::width = 0;
int Renderer::height = 0;

void Renderer::resize(int _width, int _height) {
    width = _width;
    height = _height;
    setupTexture();
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
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width * scaling, height * scaling, 0, GL_RGBA, GL_FLOAT, nullptr);
    glBindImageTexture(0, raytrace_image, 0, false, 0, GL_READ_WRITE, GL_RGBA32F);
}

void Renderer::render() {
    rayTrace();
    renderViewport();
}

void Renderer::rayTrace() {
    if (glIsProgram(raytrace_shader->getID())) {
        glUseProgram(raytrace_shader->getID());
    } else {
        std::cerr << "That wasn't a valid ray tracing program." << std::endl;
    }

    glDispatchCompute(
            ceil(width  * scaling / raytrace_work_group_dim[0]),
            ceil(height * scaling / raytrace_work_group_dim[1]),
            1
    );

}

void Renderer::renderViewport() {
    if (glIsProgram(viewport_shader->getID())) {
        glUseProgram(viewport_shader->getID());
    } else {
        std::cerr << "That wasn't a valid viewport shader program" << std::endl;
    }
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, raytrace_image);

    glBindVertexArray(viewport_vao);
    glEnableVertexAttribArray(0); // Enable vertex position data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, viewport_index_vbo);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

    // Unbind everything.
    glUseProgram(0);
    glDisableVertexAttribArray(0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glActiveTexture(0);
}

Renderer::Renderer() {
    setupViewportQuad();
    setupViewportProgram();
    setupTexture();
    setupRaytraceProgram();
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
    std::cerr << "Work group dimensions: " << raytrace_work_group_dim[0] << ", "
        << raytrace_work_group_dim[1] << ", " << raytrace_work_group_dim[2] << std::endl;
    int work_grp_cnt[3];

    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &work_grp_cnt[0]);
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1, &work_grp_cnt[1]);
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2, &work_grp_cnt[2]);

    fprintf(stderr, "max global (total) work group size x:%i y:%i z:%i\n",
           work_grp_cnt[0], work_grp_cnt[1], work_grp_cnt[2]);

    int work_grp_size[3];

    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &work_grp_size[0]);
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1, &work_grp_size[1]);
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 2, &work_grp_size[2]);

    fprintf(stderr, "max local (in one shader) work group sizes x:%i y:%i z:%i\n",
           work_grp_size[0], work_grp_size[1], work_grp_size[2]);

    int work_grp_inv;
    glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &work_grp_inv);
    fprintf(stderr, "max local work group invocations %i\n", work_grp_inv);
}

