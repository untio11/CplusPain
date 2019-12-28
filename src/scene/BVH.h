#ifndef CPLUSPAIN_BVH_H
#define CPLUSPAIN_BVH_H

#include "BoundingVolume.h"

class BVH : public BoundingVolume {
private:
public:
    unsigned int add(BoundingVolume& subvolume);
    void remove(unsigned int subvolume_id);
    float* getData();
};


#endif //CPLUSPAIN_BVH_H
