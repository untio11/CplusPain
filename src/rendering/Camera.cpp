#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <glm/gtx/string_cast.hpp>

Camera::Camera() {
    local_coords = glm::mat4(1.0f);
    position = glm::vec4(0.0f, 0.0f, -1.0f, 0);
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
        local_coords = glm::rotate(local_coords, degrees, axis);
    } else {
        local_coords = glm::rotate(local_coords, degrees, glm::mat3(local_coords) * axis);
    }
}

void Camera::translate(glm::vec3 translation, bool absolute) {
    if (absolute) {
        position += glm::vec4(translation, 0.0f);
    } else {
        position += position * glm::vec4(translation, 0.0f);
    }
}

