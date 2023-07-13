#pragma once
#include "Utils.hpp"
#include "Vector.hpp"
class onb{
    public:
        onb(){}
        inline vec3 operator[](int i) const {return axis[i];}

        vec3 u() const {return axis[0];}
        vec3 v() const {return axis[1];}
        vec3 w() const {return axis[2];}

        vec3 local(double a, double b, double c) const {return u()*a + v()*b + w()*c;}

        vec3 local(const vec3& a) const {return u()*a.x() + v()*a.y() + w()*a.z();}

        void build_from_w(const vec3&);

        vec3 axis[3];
};

void onb::build_from_w(const vec3& n) {
    axis[2] = unit_vector(n);
    vec3 a;
    if(fabs(w().x()) > 0.9)
        a = vec3(0,1,0);
    else
        a = vec3(1,0,0);
    axis[1] = unit_vector(cross(w(), a));
    axis[0] = cross(w(), v());
}