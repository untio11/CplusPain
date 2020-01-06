#include <iostream>
#include <cmath>
#include "BVHFunc.h"
#include "InnerCluster.h"
#include "../../util/MathHelp.h"

void BVHFunc::makePartition(std::vector<const Primitive*> const &prims, std::vector<const Primitive*> &left, std::vector<const Primitive*> &right, int step) {
	int i = 0;
	if (step > 29) {
		std::cout << "[Debug] Splitting in two now.\n";
		int halfway_floor = prims.size() / 2;
		for (int j = 0; j < prims.size(); j++) {
			if (j < halfway_floor) {
				left.push_back(prims[j]);
			} else {
				right.push_back(prims[j]);
			}
		}
		return;
	}
	for (auto prim = prims.begin(); prim != prims.end(); prim++) {
		if ((((*prim)->morton_code >> unsigned(29 - step)) & 1) == 1) {
			std::cout << "[Debug] Trying prims.size() = " << prims.size() << ", splitting at: " << i << std::endl;
			left.insert(left.begin(), prims.begin(), prim);
			right.insert(right.begin(), prim, prims.end());
			int left_size = left.size();
			int right_size = right.size();
			std::cout << "[Debug] Partition: (" << left_size << "," << right_size << ")\n";
			return;
		}
		i++;
	}
	// In this iteration, all primitives are on the same side.
	left.insert(left.begin(), prims.begin(), prims.end());
}

inline int BVHFunc::CCReduction(int max_clusters) {
	return std::round(max_clusters / 2.0);
}

std::vector<const Cluster*> BVHFunc::buildTree(std::vector<const Primitive*> const &prims, int stopping_threshold, int step) {
	std::cout << "[Debug] BuildTree called on " << prims.size() << " primitives in iteration " << step + 1 << std::endl;
	if (prims.size() < stopping_threshold) {
		if (prims.empty()) {
			int debug = 0;
		}
		auto result = std::vector<const Cluster*>();
		result.push_back(new LeafCluster(prims));
		return result;
	}

	std::vector<const Primitive*> left, right;
	makePartition(prims, left, right, step);
	if (left.size() + right.size() != prims.size()) std::cout << "[Fatal] Partition leaves out leaves: " <<
	left.size() + right.size() << " != " << prims.size() << ".\n";

	std::cout << "[Debug] Left tree size: " << left.size() << std::endl;
	auto left_cluster = buildTree(left, stopping_threshold, step + 1);

	std::cout << "[Debug] Right tree size: " << right.size() << std::endl;
	auto right_cluster = buildTree(right, stopping_threshold, step + 1);

	left_cluster.insert(left_cluster.end(), right_cluster.begin(), right_cluster.end());

	return CombineClusters(left_cluster, CCReduction(prims.size()));
}

std::vector<const Cluster*> BVHFunc::buildTree(std::vector<Primitive> const &prims, int stopping_threshold) {
	auto primpointers = std::vector<const Primitive*>();
	for (const auto &prim : prims) {
		primpointers.push_back(&prim);
	}
	return buildTree(primpointers, stopping_threshold, 0);
}

float BVHFunc::dissimilarity(const Cluster *c1, const Cluster *c2) {
	auto bl = MathHelp::getMin(c1->bottom_left, c2->bottom_left);
	auto tr = MathHelp::getMax(c1->top_right, c2->top_right);
	float result = 0;
	result += 2 * (tr.y - bl.y) * (tr.z - bl.z);
	result += 2 * (tr.x - bl.x) * (tr.z - bl.z);
	result += 2 * (tr.y - bl.y) * (tr.x - bl.x);
	if (result < 0) std::cout << "[Fatal] Negative surface area of bounding box\n";
	return result;
}

std::vector<const Cluster *> BVHFunc::CombineClusters(const std::vector<const Cluster*> &clusters, int max_clusters) {
	auto result = std::vector<const Cluster*>();
	result.reserve(clusters.size());
	for (const auto &cluster : clusters) {
		if (cluster->size() > 0) {
			result.push_back(cluster);
		}
	}

	for (const auto &cluster : result) {
		cluster->closest = cluster->findBestMatch(result);
	}

	while(result.size() > max_clusters) {
		float best = std::numeric_limits<float>::max();
		const Cluster *left = nullptr;
		const Cluster *right = nullptr;

		for (const auto &cluster : result) {
			if (cluster->dissimilarity != 0 && cluster->dissimilarity < best) {
				best = cluster->dissimilarity;
				left = cluster;
				right = cluster->closest;
			}
		}

		Cluster* best_merge;

		if (left != nullptr && right != nullptr) {
			best_merge = new InnerCluster({left, right});
		} else if (left == nullptr && right != nullptr) {
			best_merge = new InnerCluster({right});
		} else if (left != nullptr) {
			best_merge = new InnerCluster({left});
		}

		removeCluster(result, left);
		removeCluster(result, right);
		result.push_back(best_merge);
		best_merge->closest = best_merge->findBestMatch(result);

		for (const auto &cluster : result) {
			if (cluster->closest == left || cluster->closest == right) {
				cluster->closest = cluster->findBestMatch(result);
			}
		}
	}

	return result;
}

void BVHFunc::removeCluster(std::vector<const Cluster *> &clusters, const Cluster *cluster) {
	if (cluster == nullptr) return;
	clusters.erase(
		std::remove(
			clusters.begin(),
			clusters.end(),
			cluster
		),

		clusters.end()
	);
}
