#include "Camera.h"
#include <iostream>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/quaternion.hpp>

Camera::Camera() {
    local_coords = glm::quat(glm::mat4(1.0));
    position = glm::vec3(0.0f, 0.0f, -1.0f);
    last_x = -1;
    last_y = -1;
    front = glm::vec3(0.0, 0.0, 1.0);
    right = glm::vec3(1.0, 0.0, 0.0);
    up    = glm::vec3(0.0, 1.0, 0.0);
}

glm::vec3 Camera::getPosition() {
    return position;
}

float Camera::getCameraDistance() {
    return camera_distance;
}

glm::mat3 Camera::getRotationMatrix() {
    return glm::transpose(glm::toMat3(local_coords));
}

void Camera::rotate(glm::vec3 const& axis, float degrees) {
    glm::quat quat = glm::angleAxis(degrees, axis);
    up = quat * up;
    right = quat * right;
    front = quat * front;
    local_coords = quat * local_coords;
}

void Camera::translate(glm::vec3 translation, double delta_t) {
    position += translation * (float) delta_t * speed;
}

void Camera::cameraCallback(GLFWwindow *window, double x_pos, double y_pos) {
    static auto global_up = glm::vec3(0, 1, 0);
    if (last_x == -1) {
        last_x = x_pos;
        last_y = y_pos;
    }

    float x_offset = (x_pos - last_x) * sensitivity;
    float y_offset = (y_pos - last_y) * sensitivity ;

    rotate(global_up, x_offset);
    rotate(right, y_offset);

    last_x = x_pos;
    last_y = y_pos;
}

void Camera::zoom(double amount) {
    camera_distance = __min(10.5, __max(0.5, camera_distance + amount));
}

