#ifndef CPLUSPAIN_CAMERA_H
#define CPLUSPAIN_CAMERA_H

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat3x3.hpp>

class Camera {
private:
    float camera_distance = 1.2; // Distance from viewport. Determines FOV.
    glm::vec4 position;
    glm::mat4 local_coords;
public:
    Camera();
    glm::vec3 getPosition();
    glm::mat3 getLocalAxis();
    float getCameraDistance();
    void rotate(glm::vec3 const& axis, float degrees, bool absolute = false);
    /**
     * Translate the camera in the given direction.
     * When absolute is set to false, it will take translation as a direction to move in relative to the
     * rotation of the camera. Otherwise, it will move in world coordinates.
     * @param translation The direction in which to move.
     * @param absolute Whether or not to use world coordinates for the translation.
     */
    void translate(glm::vec3 translation, bool absolute = false);
};


#endif //CPLUSPAIN_CAMERA_H
