#define TINYOBJLOADER_IMPLEMENTATION
#include "Prop.h"
#include <iostream>
#include <glm/gtx/string_cast.hpp>

float *Prop::getData() {
    return nullptr;
}

void Prop::loadObj(const std::string& file_name) {
    std::string warn;
    std::string err;

    bool ret = tinyobj::LoadObj(&attributes, &shapes, &materials, &warn, &err, file_name.c_str());

    if (!warn.empty()) {
        std::cout << "[Warn] " << warn << std::endl;
    }

    if (!err.empty()) {
        std::cerr << "[Error] " << err << std::endl;
    }

    if (!ret) {
        exit(1);
    }

    printf("[Info] Nr. of vertices  = %d\n", (int)(attributes.vertices.size()) / 3);
    printf("[Info] Nr. of normals   = %d\n", (int)(attributes.normals.size()) / 3);
    printf("[Info] Nr. of texcoords = %d\n", (int)(attributes.texcoords.size()) / 2);
    printf("[Info] Nr. of materials = %d\n", (int)materials.size());
    printf("[Info] Nr. of shapes    = %d\n", (int)shapes.size());
    // Add default material to the back in case of emergency.
    materials.emplace_back();
    unsigned int counter = 0;
    // Go over each shape in the mesh (from triangles till whatevergons).
    for (int s = 0; s < shapes.size(); s++) {
        int index_offset = 0;

        // Loop over the faces of that shape.
        for (int f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
            // Number of vertices per shape
            int shape_verts = shapes[s].mesh.num_face_vertices[f];
            if(shape_verts != 3) {
				std::cout << "[Fatal] Fatal error, a not triangle in this mesh! " << file_name << std::endl;
				return;
			}

            // Go over each vertex in the face.
            auto center = glm::vec3(0.0);
            for (int v = 0; v < shape_verts; v++) {
                center.x += attributes.vertices[shapes[s].mesh.indices[v].vertex_index    ] / 3.0;
                center.y += attributes.vertices[shapes[s].mesh.indices[v].vertex_index + 1] / 3.0;
                center.z += attributes.vertices[shapes[s].mesh.indices[v].vertex_index + 2] / 3.0;
            }
			primitives.emplace_back(this, s, f, center);
		}
    }
}

float* Prop::getPositions() {
    return nullptr;
}

std::vector<float> Prop::getVertexPositions() {
    return attributes.vertices;
}
