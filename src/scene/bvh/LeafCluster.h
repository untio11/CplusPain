#ifndef CPLUSPAIN_LEAFCLUSTER_H
#define CPLUSPAIN_LEAFCLUSTER_H

#include <vector>
#include "Cluster.h"
#include "Primitive.h"

class LeafCluster : public Cluster {
private:
	std::vector<const Primitive*> representation = std::vector<const Primitive*>();
public:
	explicit LeafCluster(const std::vector<const Primitive*> &primitives);
	int size() const override;
	~LeafCluster();
	int getLeaves() const override;
	void printTree(int depth) const override;
};


#endif //CPLUSPAIN_LEAFCLUSTER_H
