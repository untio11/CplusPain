#ifndef CPLUSPAIN_PROP_H
#define CPLUSPAIN_PROP_H

#include "../../lib/tiny_obj_loader/tiny_obj_loader.h"
#include "BoundingVolume.h"
#include <vector>
#include <glm/vec3.hpp>
#include "bvh/Primitive.h"

class Prop {
private:
    tinyobj::attrib_t attributes;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::vector<Primitive> primitives;
public:
    float* getData();
    float* getPositions();
    void loadObj(const std::string& file_name);
    std::vector<float> getVertexPositions();
};


#endif //CPLUSPAIN_PROP_H
