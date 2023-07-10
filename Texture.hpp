#pragma once

#include "Utils.hpp"

class texture {
    public:
        virtual color3 value(double u, double v, const point3& p) const = 0;
};

class solidcolor: public texture {
    public:
        solidcolor(){}
        solidcolor(color3 c): colorValue(c) {}

        solidcolor(double red, double green, double blue): solidcolor(color3(red, green, blue)) {}

        virtual color3 value(double u, double v, const vec3& p) const override {
            return colorValue;
        }
    private:
        color3 colorValue;
};

class checkertexture: public texture{
    public:
        checkertexture(){}
        checkertexture(shared_ptr<texture> even, shared_ptr<texture> odd): even(even), odd(odd) {}
        checkertexture(color3 c1, color3 c2): even(make_shared<solidcolor>(c1)), odd(make_shared<solidcolor>(c2)) {}

        virtual color3 value(double u, double v, const point3& p) const override {
            auto sines = sin(10*p.x()) * sin(10*p.y()) * sin(10*p.z());
            if(sines < 0)
                return odd->value(u, v, p);
            else
                return even->value(u, v, p);
        }
    public:
        shared_ptr<texture> odd;
        shared_ptr<texture> even;
};