#include <limits>
#include "Cluster.h"
#include "BVHFunc.h"

const Cluster* Cluster::findBestMatch(const std::vector<const Cluster*> &clusters) const {
	float min_diss = std::numeric_limits<float>::max();
	const Cluster* result = nullptr;

	for (auto &cluster : clusters) {
		if (cluster == this) continue;
		float diss = BVHFunc::dissimilarity(this, cluster);
		if (diss < min_diss) {
			min_diss = diss;
			this->dissimilarity = min_diss;
			result = cluster;
		}
	}

	if (result == nullptr) {
		int debug = 0;
	}

	return result;
}