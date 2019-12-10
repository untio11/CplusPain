//
// Created by robin on 03/12/2019.
//

#include <fstream>
#include <sstream>
#include <iostream>
#include "ShaderProgram.h"
#include "../../lib/glad/include/glad/glad.h"

void ShaderProgram::use() {
    glUseProgram(ID);
}

void ShaderProgram::addFile(std::string file_path, GLenum type) {
    std::string code;
    std::ifstream shader_file;
    shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        shader_file.open(file_path);

        std::stringstream shader_stream;
        shader_stream << shader_file.rdbuf();
        shader_file.close();
        code = shader_stream.str();
    } catch (std::ifstream::failure& e) {
        std::cerr << "Shader file " << file_path << " not read successfully." << std::endl;
        std::cerr << e.what() << std::endl;
    }
    files.emplace_back(code, type);
}

std::vector<std::pair<unsigned int, GLenum>> ShaderProgram::compileShaders() {
    std::vector<std::pair<unsigned int, GLenum>> IDs;
    int success;
    char info_log[512];

    for (const auto &file : files) {
        unsigned int shader_id;
        shader_id = glCreateShader(file.second);
        auto c_text = file.first.c_str();
        glShaderSource(shader_id, 1, &c_text, nullptr);
        glCompileShader(shader_id);
        glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader_id, 512, nullptr, info_log);
            std::cerr << "Shader compilation failed: " << info_log << std::endl;
        }

        IDs.emplace_back(shader_id, file.second);
    }

    return IDs;
}

unsigned int ShaderProgram::linkShaders(std::vector<std::pair<unsigned int, GLenum>> shaders, unsigned int program_id) {
    ID = program_id;
    int success;
    char info_log[512];

    for (auto shader : shaders) {
        glAttachShader(ID, shader.first);
    }
    glLinkProgram(ID);
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(ID, 512, NULL, info_log);
        std::cerr << "Shader linking failed: " << info_log << std::endl;
    }

    for (auto shader : shaders) {
        glDeleteShader(shader.first);
    }

    return ID;
}

unsigned int ShaderProgram::createProgram() {
    if (!files.empty()) {
        return linkShaders(compileShaders(), glCreateProgram());
    }
    return 0;
}

ShaderProgram::~ShaderProgram() {
    glDeleteProgram(ID);
}
