#ifndef CPLUSPAIN_CAMERA_H
#define CPLUSPAIN_CAMERA_H

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat3x3.hpp>
#include <glm/gtc/quaternion.hpp>

class GLFWwindow;

class Camera {
private:
    float camera_distance = 1.2; // Distance from viewport. Determines FOV.
    glm::vec3 position;
    glm::quat local_coords;
    double last_x, last_y;
    float sensitivity = 0.003;
    float speed = 0.05;
public:
    glm::vec3 front, right, up;
    Camera();
    glm::vec3 getPosition();
    glm::mat3 getRotationMatrix();
    float getCameraDistance();
    void rotate(glm::vec3 const& axis, float degrees);
    /**
     * Translate the camera in the given direction.
     * @param translation The direction in which to move.
     */
    void translate(glm::vec3 translation, double delta_t);
    void cameraCallback(GLFWwindow* window, double x_pos, double y_pos);
    /**
     * Change the fov of the camera to zoom in or out.
     * @param amount The amount with which to change the FOV. Negative values represent zooming out and positive zooming in.
     */
    void zoom(double amount);
};


#endif //CPLUSPAIN_CAMERA_H
