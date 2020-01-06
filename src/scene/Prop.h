#ifndef CPLUSPAIN_PROP_H
#define CPLUSPAIN_PROP_H

#include "../../lib/tiny_obj_loader/tiny_obj_loader.h"
#include <vector>
#include <glm/vec3.hpp>
#include "bvh/Primitive.h"
#include "bvh/Cluster.h"

class Prop {
private:
    tinyobj::attrib_t attributes;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::vector<Primitive> primitives;
public:
    void loadObj(const std::string& file_name);
};


#endif //CPLUSPAIN_PROP_H
