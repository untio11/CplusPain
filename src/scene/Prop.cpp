#define TINYOBJLOADER_IMPLEMENTATION
#include "Prop.h"
#include <iostream>
#include <algorithm>
#include "bvh/BVHFunc.h"
#include "bvh/LeafCluster.h"

void Prop::loadObj(const std::string& file_name) {
    std::string warn;
    std::string err;

    bool ret = tinyobj::LoadObj(&attributes, &shapes, &materials, &warn, &err, file_name.c_str());
    if (!warn.empty())
        std::cout << "[Warn] " << warn << std::endl;

    if (!err.empty())
        std::cerr << "[Error] " << err << std::endl;

    if (!ret)
        exit(1);

    printf("[Info] Nr. of vertices  = %d\n", (int)(attributes.vertices.size()) / 3);
    printf("[Info] Nr. of normals   = %d\n", (int)(attributes.normals.size()) / 3);
    printf("[Info] Nr. of texcoords = %d\n", (int)(attributes.texcoords.size()) / 2);
    printf("[Info] Nr. of materials = %d\n", (int)materials.size());
    printf("[Info] Nr. of shapes    = %d\n", (int)shapes.size());
    // Add default material to the back in case of emergency.
    materials.emplace_back();

    auto min = glm::vec3(std::numeric_limits<float>::max());
	auto max = glm::vec3(-std::numeric_limits<float>::max());

    // Go over each shape in the mesh (from triangles till whatevergons).
    for (int s = 0; s < shapes.size(); s++) {
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
                center.x += attributes.vertices[shapes[s].mesh.indices[shape_verts * f + v].vertex_index    ] / 3.0;
                center.y += attributes.vertices[shapes[s].mesh.indices[shape_verts * f + v].vertex_index + 1] / 3.0;
                center.z += attributes.vertices[shapes[s].mesh.indices[shape_verts * f + v].vertex_index + 2] / 3.0;
            }
			primitives.emplace_back(this, s, f, center);
			min.x = std::min(center.x, min.x);
			min.y = std::min(center.y, min.y);
			min.z = std::min(center.z, min.z);
			max.x = std::max(center.x, max.x);
			max.y = std::max(center.y, max.y);
			max.z = std::max(center.z, max.z);
		}
    }

    for (auto& prim : primitives) {
		prim.center.x = (prim.center.x - min.x) / (max.x - min.x);
		prim.center.y = (prim.center.y - min.y) / (max.y - min.y);
		prim.center.z = (prim.center.z - min.z) / (max.z - min.z);
		prim.morton_code = Morton::morton3D(prim.center.x, prim.center.y, prim.center.z);
	}

    std::sort(primitives.begin(), primitives.end());

    for (int i = 0; i < primitives.size(); i++) {
    	primitives[i].index = i;
    }
    std::cout << "[Debug] Primitives.size() = " << primitives.size() << std::endl;

    auto result = BVHFunc::CombineClusters(BVHFunc::buildTree(primitives, 6), 1);
    std::cout << "[Debug] Final cluster size: " << result.size() << std::endl;
    std::cout << "[Debug] Leaf count: " << result[0]->getLeaves() << ((result[0]->getLeaves() == primitives.size()) ? " == " : " != ") << primitives.size() << std::endl;
    result[0]->printTree(0);
}
