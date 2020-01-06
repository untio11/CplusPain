#include "LeafCluster.h"
#include <limits>
#include <iostream>

LeafCluster::LeafCluster(const std::vector<const Primitive*> &primitives) {
	this->type = ClusterType::LEAF;
	for (const auto &prim : primitives) {
		representation.push_back(prim);
	}

	bottom_left = glm::vec3(std::numeric_limits<float>::max());
	top_right =   glm::vec3(-std::numeric_limits<float>::max());

	for (const auto &prim : representation) {
		bottom_left.x = std::min(prim->center.x, bottom_left.x);
		bottom_left.y = std::min(prim->center.y, bottom_left.y);
		bottom_left.z = std::min(prim->center.z, bottom_left.z);
		top_right.x =   std::max(prim->center.x, top_right.x);
		top_right.y =   std::max(prim->center.y, top_right.y);
		top_right.z =   std::max(prim->center.z, top_right.z);
	}
}

int LeafCluster::size() const {
	return representation.size();
}

LeafCluster::~LeafCluster() {
	for (auto & i : representation) {
		delete i;
	}
}

int LeafCluster::getLeaves() const {
	return representation.size();
}

void LeafCluster::printTree(int depth) const {
	for (const auto &prim : representation) {
		for (int i = 0; i < depth; i++) {
			std::cout << " ";
		}
		std::cout << "<" << depth << ":" << prim->index << ":" << prim->morton_code << ">\n";
	}
}
