#include "InnerCluster.h"
#include <algorithm>
#include <iostream>

InnerCluster::InnerCluster(const std::vector<const Cluster *> &clusters) {
	this->type = ClusterType::INNER;
	bottom_left = glm::vec3(std::numeric_limits<float>::max());
	top_right = glm::vec3(-std::numeric_limits<float>::max());

	for (const auto &cluster : clusters) {
		representation.push_back(cluster);

		bottom_left.x = std::min(cluster->bottom_left.x, bottom_left.x);
		bottom_left.y = std::min(cluster->bottom_left.y, bottom_left.y);
		bottom_left.z = std::min(cluster->bottom_left.z, bottom_left.z);
		top_right.x = std::max(cluster->top_right.x, top_right.x);
		top_right.y = std::max(cluster->top_right.y, top_right.y);
		top_right.z = std::max(cluster->top_right.z, top_right.z);
	}
}

int InnerCluster::size() const {
	return representation.size();
}

InnerCluster::~InnerCluster() {
	for (auto & i : representation) {
		delete i;
	}
}

int InnerCluster::getLeaves() const {
	int count = 0;
	for (auto &child : representation) {
		count += child->getLeaves();
	}
	return count;
}

void InnerCluster::printTree(int depth) const {
	for (const auto &child : representation) {
		for (int i = 0; i < depth; i++) {
			std::cout << " ";
		}
		std::cout << "[" << depth << ":" << representation.size() << ":" << child->getLeaves() << "]\n";
		child->printTree(depth + 1);
	}
}