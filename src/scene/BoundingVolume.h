#ifndef CPLUSPAIN_BOUNDINGVOLUME_H
#define CPLUSPAIN_BOUNDINGVOLUME_H

#include <vector>

class BoundingVolume {
private:
    std::vector<BoundingVolume*> children;
protected:
public:
    int add(BoundingVolume& child);
    void remove(int child_id);
};

#endif //CPLUSPAIN_BOUNDINGVOLUME_H
