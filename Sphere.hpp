#pragma once

#include "Vector.hpp"
#include "Hitable.hpp"

class sphere : public hitable{
    public:
        sphere(){}
        sphere(point3 cen, double r, shared_ptr<material> m) : center(cen), radius(r), matPtr(m) {};

        virtual bool hit(const ray& r, double t_min, double t_max, hitrecord& rec) const override;
        virtual bool boundingBox(double time0, double time1, aabb& output_box) const override;
        static void getSphereUV(const point3& p, double& u, double& v){
            auto theta = acos(-p.y());
            auto phi = atan2(-p.z(), p.x()) + pi;

            u = phi / (2*pi);
            v = theta / pi;
        }

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
    sphere::getSphereUV(outwardNormal, rec.u, rec.v);
    rec.matPtr = matPtr;
    
    return true;
}

bool sphere::boundingBox(double time0, double time1, aabb& output_box) const{
    output_box = aabb(center - vec3(radius, radius, radius), center + vec3(radius, radius, radius));
    return true;
}

class moveing_sphere: public hitable{
    public:
        moveing_sphere(){}
        moveing_sphere(point3 cen0, point3 cen1, double t0, double t1, double r, shared_ptr<material> m) : center0(cen0), center1(cen1), time0(t0), time1(t1), radius(r), matPtr(m) {};

        virtual bool hit(const ray& r, double t_min, double t_max, hitrecord& rec) const override;
        virtual bool boundingBox(double time0, double time1, aabb& output_box) const override;

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

bool moveing_sphere::hit(const ray& r, double t_min, double t_max, hitrecord& rec) const {

    vec3 oc = r.origin() - center(r.time());
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
    auto outwardNormal = (rec.p - center(r.time())) / radius;
    rec.setFaceNormal(r, outwardNormal);
    rec.matPtr = matPtr;
    
    return true;
}

bool moveing_sphere::boundingBox(double time0, double time1, aabb& output_box) const{
    aabb box0(center(time0) - vec3(radius, radius, radius), center(time0) + vec3(radius, radius, radius));
    aabb box1(center(time1) - vec3(radius, radius, radius), center(time1) + vec3(radius, radius, radius));
    output_box = surroundingBox(box0, box1);
    return true;
}