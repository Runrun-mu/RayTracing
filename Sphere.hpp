#pragma once

#include "Vector.hpp"
#include "Hitable.hpp"

class sphere : public hitable{
    public:
        sphere(){}
        sphere(point3 cen, double r) : center(cen), radius(r) {};

        virtual bool hit(const ray& r, double t_min, double t_max, hitrecord& rec) const override;

    public:
        point3 center;
        double radius;
};

bool sphere::hit(const ray& r, double t_min, double t_max, hitrecord& rec) const{
    vec3 oc = r.origin() - center;
    auto a = dot(r.direction(), r.direction());
    auto b = dot(oc, r.direction()) * 2;
    auto c = dot(oc,oc) - radius * radius;
    auto discriminant = b*b - 4*a*c;
    if(discriminant < 0)
        return false;
    auto t = (- b - sqrt(discriminant)) / (2.0 * a);
    if(t < t_min || t > t_max){
        t = (- b + sqrt(discriminant)) / (2.0 * a);
        if(t < t_min || t > t_max)
            return false;
    }
    rec.t = t;
    rec.p = r.at(t);
    rec.normal = unit_vector(rec.p - center);
    return true;
}