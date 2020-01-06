#ifndef CPLUSPAIN_CLUSTER_H
#define CPLUSPAIN_CLUSTER_H

#include <glm/vec3.hpp>
#include <vector>
#include <string>

enum ClusterType {
	LEAF,
	INNER
};

class Cluster {
public:
	const Cluster mutable* closest;
	mutable float dissimilarity;
	glm::vec3 bottom_left;
	glm::vec3 top_right;
	const Cluster* findBestMatch(const std::vector<const Cluster*> &clusters) const;
	virtual int getLeaves() const = 0;
	virtual void printTree(int depth) const = 0;
	ClusterType type;
	virtual int size() const = 0;
};


#endif //CPLUSPAIN_CLUSTER_H