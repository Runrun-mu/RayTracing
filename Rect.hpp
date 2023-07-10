#pragma once

#include "Utils.hpp"
#include "Hitable.hpp"

class xyrect : public hitable {
    public:
        xyrect(){}
        xyrect(double x0, double x1, double y0, double y1, double k, shared_ptr<material> mat)
            : x0(x0), x1(x1), y0(y0), y1(y1), k(k), matPtr(mat)
        {}

        virtual bool hit(const ray& r, double t_min, double t_max, hitrecord& rec) const override;

        virtual bool boundingBox(double time0, double time1, aabb& outputBox) const override{
            outputBox = aabb(point3(x0, y0, k - 0.0001), point3(x1, y1, k + 0.0001));
            return true;
        }

    public:
        shared_ptr<material> matPtr;
        double x0, x1, y0, y1, k;
};

bool xyrect::hit(const ray& r, double t_min, double t_max, hitrecord& rec) const {
    auto t = (k - r.origin().z()) / r.direction().z();
    if(t < t_min || t > t_max)
        return false;
    auto x = r.origin().x() + t * r.direction().x();
    auto y = r.origin().y() + t * r.direction().y();
    if(x < x0 || x > x1 || y < y0 || y > y1)
        return false;
    rec.u = (x - x0) / (x1 - x0);
    rec.v = (y - y0) / (y1 - y0);
    rec.t = t;
    auto outwardNormal = vec3(0, 0, 1);
    rec.setFaceNormal(r, outwardNormal);
    rec.matPtr = matPtr;
    rec.p = r.at(t);

    return true;
}

class xzrect: public hitable {
    public:
        xzrect(){}
        xzrect(double x0, double x1, double z0, double z1, double k, shared_ptr<material> mat)
            : x0(x0), x1(x1), z0(z0), z1(z1), k(k), matPtr(mat)
        {}

        virtual bool hit(const ray& r, double t_min, double t_max, hitrecord& rec) const override;

        virtual bool boundingBox(double time0, double time1, aabb& outputBox) const override{
            outputBox = aabb(point3(x0, k - 0.0001, z0), point3(x1, k + 0.0001, z1));
            return true;
        }

    public:
        shared_ptr<material> matPtr;
        double x0, x1, z0, z1, k;
};

bool xzrect::hit(const ray& r, double t_min, double t_max, hitrecord& rec) const {
    auto t = (k-r.origin().y()) / r.direction().y();
    if (t < t_min || t > t_max)
        return false;
    auto x = r.origin().x() + t*r.direction().x();
    auto z = r.origin().z() + t*r.direction().z();
    if (x < x0 || x > x1 || z < z0 || z > z1)
        return false;
    rec.u = (x-x0)/(x1-x0);
    rec.v = (z-z0)/(z1-z0);
    rec.t = t;
    auto outwardNormal = vec3(0, 1, 0);
    rec.setFaceNormal(r, outwardNormal);
    rec.matPtr = matPtr;
    rec.p = r.at(t);
    return true;
}

class yzrect: public hitable{
    public:
        yzrect(){}
        yzrect(double y0, double y1, double z0, double z1, double k, shared_ptr<material> mat)
            : y0(y0), y1(y1), z0(z0), z1(z1), k(k), matPtr(mat)
        {}

        virtual bool hit(const ray& r, double t_min, double t_max, hitrecord& rec) const override;

        virtual bool boundingBox(double time0, double time1, aabb& outputBox) const override{
            outputBox = aabb(point3(k - 0.0001, y0, z0), point3(k + 0.0001, y1, z1));
            return true;
        }

    public:
        shared_ptr<material> matPtr;
        double y0, y1, z0, z1, k;
};

bool yzrect::hit(const ray& r, double t_min, double t_max, hitrecord& rec) const {
    auto t = (k-r.origin().x()) / r.direction().x();
    if (t < t_min || t > t_max)
        return false;
    auto y = r.origin().y() + t*r.direction().y();
    auto z = r.origin().z() + t*r.direction().z();
    if (y < y0 || y > y1 || z < z0 || z > z1)
        return false;
    rec.u = (y-y0)/(y1-y0);
    rec.v = (z-z0)/(z1-z0);
    rec.t = t;
    auto outwardNormal = vec3(1, 0, 0);
    rec.setFaceNormal(r, outwardNormal);
    rec.matPtr = matPtr;
    rec.p = r.at(t);
    return true;
}