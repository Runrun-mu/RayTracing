#pragma once

#include "Vector.hpp"
#include "Hitable.hpp"

class sphere : public hitable{
    public:
        sphere(){}
        sphere(point3 cen, double r, shared_ptr<material> m) : center(cen), radius(r), matPtr(m) {};

        virtual bool hit(const ray& r, double t_min, double t_max, hitrecord& rec) const override;

    public:
        point3 center;
        double radius;
        shared_ptr<material> matPtr;
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
    vec3 outwardNormal = (rec.p - center) / radius;
    rec.setFaceNormal(r, outwardNormal);
    rec.matPtr = matPtr;
    
    return true;
}

class moveing_sphere: public hitable{
    public:
        moveing_sphere(){}
        moveing_sphere(point3 cen0, point3 cen1, double t0, double t1, double r, shared_ptr<material> m) : center0(cen0), center1(cen1), time0(t0), time1(t1), radius(r), matPtr(m) {};

        virtual bool hit(const ray& r, double t_min, double t_max, hitrecord& rec) const override;

        point3 center(double time) const;

    public:
        point3 center0, center1;
        double time0, time1;
        double radius;
        shared_ptr<material> matPtr;
};

point3 moveing_sphere::center(double time) const{
    return center0 + (center1 - center0) * ((time - time0) / (time1 - time0));
}