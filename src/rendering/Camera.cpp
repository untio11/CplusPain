#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

Camera::Camera() {
    local_coords = glm::quat();
    position = glm::vec3(0.0f, 0.0f, -1.0f);
    last_x = -1;
    last_y = -1;
    front = glm::vec3(0.0, 0.0, 1.0);
    right = glm::vec3(1.0, 0.0, 0.0);
    up = glm::vec3(0.0, 1.0, 0.0);
}

glm::vec3 Camera::getPosition() {
    return position;
}

float Camera::getCameraDistance() {
    return camera_distance;
}

glm::mat3 Camera::getRotationMatrix() {
    return glm::toMat3(local_coords);
}

void Camera::rotate(glm::vec3 const& axis, float degrees, bool absolute) {
    glm::quat quat;

    if (absolute) {
        quat = glm::angleAxis(degrees, axis);
    } else {
        quat = glm::angleAxis(degrees, axis);
        up = quat * up;
        right = quat * right;
        front = quat * front;
    }
    local_coords = quat * local_coords;
}

void Camera::translate(glm::vec3 translation, bool absolute) {
    if (absolute) {
        position += translation;
    } else {
        position += translation;
    }
}

void Camera::cameraCallback(GLFWwindow *window, double x_pos, double y_pos) {
    if (last_x == -1) {
        last_x = x_pos;
        last_y = y_pos;
    }

    float x_offset = (last_x - x_pos) * sensitivity;
    float y_offset = (last_y - y_pos) * sensitivity ;

    std::cerr << "Up is: " << glm::to_string(up) << std::endl;
    rotate(up,    x_offset, false);
    rotate(right, y_offset, false);

    last_x = x_pos;
    last_y = y_pos;

}

