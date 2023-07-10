#pragma once

#include "Utils.hpp"
#include "Hitable.hpp"
#include "Hitablelist.hpp"
#include <algorithm>

class bvhnode: public hitable {
    public:
        bvhnode(){}

        bvhnode(hitablelist& list, double time0, double time1)
            : bvhnode(list.objects, 0, list.objects.size(), time0, time1) {}

        bvhnode(std::vector<shared_ptr<hitable>>& srcObjects, size_t start, size_t end, double time0, double time1);

        virtual bool hit(const ray& r, double t_min, double t_max, hitrecord& rec) const override;

        virtual bool boundingBox(double time0, double time1, aabb& outputBox) const override;

    public:
        shared_ptr<hitable> left;
        shared_ptr<hitable> right;
        aabb box;
};

bool bvhnode::boundingBox(double time0, double time1, aabb& outputBox) const {
    outputBox = box;
    return true;
}

bool bvhnode::hit(const ray& r, double t_min, double t_max, hitrecord& rec) const {
    if(!box.hit(r, t_min, t_max))
        return false;

    bool hitLeft = left->hit(r, t_min, t_max, rec);
    bool hitRight = right->hit(r, t_min, hitLeft ? rec.t : t_max, rec);

    return hitLeft || hitRight;
}

bool boxCompare(const shared_ptr<hitable> a, const shared_ptr<hitable> b, int axis){
    aabb boxLeft, boxRight;

    if(!a->boundingBox(0, 0, boxLeft) || !b->boundingBox(0, 0, boxRight))
        std::cerr << "No bounding box in bvhnode constructor.\n";

    return boxLeft.min().e[axis] < boxRight.min().e[axis];
}

bool boxXCompare(const shared_ptr<hitable> a, const shared_ptr<hitable> b){
    return boxCompare(a, b, 0);
}

bool boxYCompare(const shared_ptr<hitable> a, const shared_ptr<hitable> b){
    return boxCompare(a, b, 1);
}

bool boxZCompare(const shared_ptr<hitable> a, const shared_ptr<hitable> b){
    return boxCompare(a, b, 2);
}

bvhnode::bvhnode(std::vector<shared_ptr<hitable>>& srcObjects, size_t start, size_t end, double time0, double time1){
    auto objects = srcObjects;

    int axis = randomInt(0, 2);
    auto comparator = (axis == 0) ? boxXCompare
                    : (axis == 1) ? boxYCompare
                    : boxZCompare;

    size_t objectSpan = end - start;

    if(objectSpan == 1){
        left = right = objects[start];
    } else if(objectSpan == 2){
        if(comparator(objects[start], objects[start + 1])){
            left = objects[start];
            right = objects[start + 1];
        } else {
            left = objects[start + 1];
            right = objects[start];
        }
    } else {
        std::sort(objects.begin() + start, objects.begin() + end, comparator);

        auto mid = start + objectSpan / 2;
        left = make_shared<bvhnode>(objects, start, mid, time0, time1);
        right = make_shared<bvhnode>(objects, mid, end, time0, time1);
    }

    aabb boxLeft, boxRight;

    if(!left->boundingBox(time0, time1, boxLeft) || !right->boundingBox(time0, time1, boxRight))
        std::cerr << "No bounding box in bvhnode constructor.\n";

    box = surroundingBox(boxLeft, boxRight);
}

