# pragma once

#include "Ray.hpp"
#include "Utils.hpp"
#include "AABB.hpp"
class material;

using std::shared_ptr;
struct hitrecord{
    point3 p;
    vec3 normal;
    double t;
    bool frontFace;
    shared_ptr<material> matPtr;
    double u;
    double v;

    inline void setFaceNormal(const ray& r, const vec3& outwardNormal){
        frontFace = dot(r.direction(), outwardNormal) < 0;
        normal = frontFace ? outwardNormal : -outwardNormal;
    }
};

class hitable{
    public:
        virtual bool hit(const ray& r, double t_min, double t_max, hitrecord& rec) const = 0;
        virtual bool boundingBox(double time0, double time1, aabb& outputBox) const = 0;
};

class translate: public hitable {
    public:
        translate(shared_ptr<hitable> p, const vec3& displacement)
            : ptr(p), offset(displacement)
        {}

        virtual bool hit(const ray& r, double t_min, double t_max, hitrecord& rec) const override;
        virtual bool boundingBox(double time0, double time1, aabb& outputBox) const override;

    public:
        shared_ptr<hitable> ptr;
        vec3 offset;
};

bool translate::hit(const ray& r, double t_min, double t_max, hitrecord& rec) const {
    ray movedR(r.origin() - offset, r.direction(), r.time());
    if(!ptr->hit(movedR, t_min, t_max, rec))
        return false;
    rec.p += offset;
    rec.setFaceNormal(movedR, rec.normal);
    return true;
}

bool translate::boundingBox(double time0, double time1, aabb& outputBox) const {
    if(!ptr->boundingBox(time0, time1, outputBox))
        return false;
    outputBox = aabb(outputBox.min() + offset, outputBox.max() + offset);
    return true;
}

class rotatey: public hitable {
    public:
        rotatey(shared_ptr<hitable> p, double angle);

        virtual bool hit(const ray& r, double t_min, double t_max, hitrecord& rec) const override;
        virtual bool boundingBox(double time0, double time1, aabb& outputBox) const override {
            outputBox = bbox;
            return hasbox;
        }

    public:
        shared_ptr<hitable> ptr;
        double sinTheta;
        double cosTheta;
        bool hasbox;
        aabb bbox;
};

rotatey::rotatey(shared_ptr<hitable> p, double angle) : ptr(p){
    auto radians = degree2Radians(angle);
    sinTheta = sin(radians);
    cosTheta = cos(radians);
    hasbox = ptr->boundingBox(0, 1, bbox);

    point3 min(infinity, infinity, infinity);
    point3 max(-infinity, -infinity, -infinity);

    for(int i = 0; i < 2; ++i)
        for(int j = 0; j < 2; ++j)
            for(int k = 0; k < 2; ++k){
                auto x = i * bbox.max().x() + (1 - i) * bbox.min().x();
                auto y = j * bbox.max().y() + (1 - j) * bbox.min().y();
                auto z = k * bbox.max().z() + (1 - k) * bbox.min().z();

                auto newX = cosTheta * x + sinTheta * z;
                auto newZ = -sinTheta * x + cosTheta * z;

                vec3 tester(newX, y, newZ);

                for(int c = 0; c < 3; ++c){
                    min[c] = fmin(min[c], tester[c]);
                    max[c] = fmax(max[c], tester[c]);
                }
            }
    bbox = aabb(min, max);
}

bool rotatey::hit(const ray& r, double tmin, double tmax, hitrecord& rec) const {
    auto origin = r.origin();
    auto direction = r.direction();

    origin[0] = cosTheta * r.origin()[0] - sinTheta * r.origin()[2];
    origin[2] = sinTheta * r.origin()[0] + cosTheta * r.origin()[2];

    direction[0] = cosTheta * r.direction()[0] - sinTheta * r.direction()[2];
    direction[2] = sinTheta * r.direction()[0] + cosTheta * r.direction()[2];

    ray rotatedR(origin, direction, r.time());

    if(!ptr->hit(rotatedR, tmin, tmax, rec))
        return false;
    
    auto p = rec.p;
    auto normal = rec.normal;

    p[0] = cosTheta * rec.p[0] + sinTheta * rec.p[2];
    p[2] = -sinTheta * rec.p[0] + cosTheta * rec.p[2];

    normal[0] = cosTheta * rec.normal[0] + sinTheta * rec.normal[2];
    normal[2] = -sinTheta * rec.normal[0] + cosTheta * rec.normal[2];

    rec.p = p;
    rec.setFaceNormal(rotatedR, normal);

    return true;
}

class flipface: public hitable{
    public:
        flipface(shared_ptr<hitable> p): ptr(p) {}

        virtual bool hit(const ray& r, double t_min, double t_max, hitrecord& rec) const override{
            if(!ptr->hit(r, t_min, t_max, rec))
                return false;
            rec.frontFace = !rec.frontFace;
            return true;
        }

        virtual bool boundingBox(double time0, double time1, aabb& outputBox) const override{
            return ptr->boundingBox(time0, time1, outputBox);
        }

        shared_ptr<hitable> ptr;
};