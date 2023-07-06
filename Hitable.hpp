#pragma once

#include "Ray.hpp"

struct hitrecord {
    point3 p;
    vec3 normal;
    double t;
    bool frontFace;

    inline void setFaceNormal(const ray& r, const vec3& outwardNormal){
        frontFace = dot(r.direction(), outwardNormal) < 0;
        normal = frontFace ? outwardNormal : -outwardNormal;
    }
};

class hitable {
    public:
        virtual bool hit(const ray& r, double t_min, double t_max, hitrecord& rec) const = 0;
};
