#ifndef CPLUSPAIN_BVHFUNC_H
#define CPLUSPAIN_BVHFUNC_H

#include <vector>
#include "Cluster.h"
#include "Primitive.h"
#include "LeafCluster.h"

class BVHFunc {
private:
	static std::vector<const Cluster*> buildTree(std::vector<const Primitive*> const &prims, int stopping_threshold, int step);
	static void removeCluster(std::vector<const Cluster*> &clusters, const Cluster* cluster);
public:
	// TODO: No need for copy when using pointers into prims.
	static void makePartition(
		std::vector<const Primitive*> const &prims,
		std::vector<const Primitive*> &left,
		std::vector<const Primitive*> &right,
		int step
	);
	static int CCReduction(int max_clusters);
	static std::vector<const Cluster*> CombineClusters(const std::vector<const Cluster*>& clusters, int max_clusters);
	static std::vector<const Cluster*> buildTree(std::vector<Primitive> const &prims, int stopping_threshold);
	static float dissimilarity(const Cluster *c1, const Cluster *c2);
};

#endif //CPLUSPAIN_BVHFUNC_H
