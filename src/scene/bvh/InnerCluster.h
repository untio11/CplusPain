#ifndef CPLUSPAIN_INNERCLUSTER_H
#define CPLUSPAIN_INNERCLUSTER_H

#include <vector>
#include "Cluster.h"

class InnerCluster : public Cluster {
private:
	std::vector<const Cluster*> representation = std::vector<const Cluster*>();
public:
	explicit InnerCluster(const std::vector<const Cluster*>& clusters);
	InnerCluster() = default;
	int size() const override;
	~InnerCluster();
	int getLeaves() const override;
	void printTree(int depth) const override;
};


#endif //CPLUSPAIN_INNERCLUSTER_H
