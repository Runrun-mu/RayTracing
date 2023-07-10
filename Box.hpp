#pragma once

#include "Utils.hpp"
#include "Hitable.hpp"
#include "Rect.hpp"
#include "Hitablelist.hpp"

class box: public hitable {
    public:
        box(){}
        box(const point3& p0, const point3& p1, shared_ptr<material> ptr);

        virtual bool hit(const ray& r, double t_min, double t_max, hitrecord& rec) const override;

        virtual bool boundingBox(double time0, double time1, aabb& outputBox) const override{
            outputBox = aabb(boxMin, boxMax);
            return true;
        }

    public:
        point3 boxMin;
        point3 boxMax;
        hitablelist sides;
};

box::box(const point3& p0, const point3& p1, shared_ptr<material> ptr){
    boxMin = p0;
    boxMax = p1;

    sides.add(make_shared<xyrect>(p0.x(), p1.x(), p0.y(), p1.y(), p1.z(), ptr));
    sides.add(make_shared<xyrect>(p0.x(), p1.x(), p0.y(), p1.y(), p0.z(), ptr));

    sides.add(make_shared<xzrect>(p0.x(), p1.x(), p0.z(), p1.z(), p1.y(), ptr));
    sides.add(make_shared<xzrect>(p0.x(), p1.x(), p0.z(), p1.z(), p0.y(), ptr));

    sides.add(make_shared<yzrect>(p0.y(), p1.y(), p0.z(), p1.z(), p1.x(), ptr));
    sides.add(make_shared<yzrect>(p0.y(), p1.y(), p0.z(), p1.z(), p0.x(), ptr));
}

bool box::hit(const ray& r, double t_min, double t_max, hitrecord& rec) const {
    return sides.hit(r, t_min, t_max, rec);
}