#ifndef CPLUSPAIN_CAMERA_H
#define CPLUSPAIN_CAMERA_H


class Camera {
private:
    float camera_distance = 1.7; // Distance from viewport. Determines FOV.
public:
    static constexpr float viewport[16] {
        -1.0f,  1.0f, 0.0f, 1.0f, // 1/6 -> ID:0
        -1.0f, -1.0f, 0.0f, 1.0f, // 2   -> ID:1
         1.0f, -1.0f, 0.0f, 1.0f, // 3/4 -> ID:2
         1.0f,  1.0f, 0.0f, 1.0f, // 5   -> ID:3
    };

    static constexpr int viewport_indices[6] {
        0, 1, 2,
        2, 3, 0
    };
};


#endif //CPLUSPAIN_CAMERA_H
