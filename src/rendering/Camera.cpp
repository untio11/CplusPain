#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <glm/gtx/string_cast.hpp>

Camera::Camera() {
    local_coords = glm::mat4(1.0f);
    position = glm::vec4(0.0f, 0.0f, -1.0f, 0);
    last_x = -1;
    last_y = -1;
    pitch = 0;
    yaw = 0;
}

glm::vec3 Camera::getPosition() {
    return position;
}

float Camera::getCameraDistance() {
    return camera_distance;
}

glm::mat3 Camera::getLocalAxis() {
    return local_coords;
}

void Camera::rotate(glm::vec3 const& axis, float degrees, bool absolute) {
    if (absolute) {
        local_coords = glm::rotate(local_coords, glm::radians(degrees), axis);
    } else {
        local_coords = glm::rotate(local_coords, glm::radians(degrees), glm::mat3(local_coords) * axis);
    }
}

void Camera::translate(glm::vec3 translation, bool absolute) {
    if (absolute) {
        position += glm::vec4(translation, 0.0f);
    } else {
        position += local_coords * glm::vec4(translation, 0.0f);
    }
}

void Camera::cameraCallback(GLFWwindow *window, double x_pos, double y_pos) {
    if (last_x == -1) {
        last_x = x_pos;
        last_y = y_pos;
    }

    float x_offset = (x_pos - last_x) * sensitivity;
    float y_offset = (last_y - y_pos) * sensitivity ;
    std::cerr << "x_offset: " << x_offset << " y_offset: " << y_offset << std::endl;
    // Clamp pitch between -89 and 89 degrees.
    float pitch = (y_offset > 89.0f) ? 89.0f : (y_offset) < -89.0f ? -89.0f : y_offset;
    rotate(glm::vec3(1.0, 0.0, 0.0), -pitch, false);
    rotate(glm::vec3(0.0, 1.0, 0.0), -x_offset, false);
    last_x = x_pos;
    last_y = y_pos;

}

