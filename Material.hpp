#pragma once

#include "Utils.hpp"

struct hitrecord;

class material{
    public:
        virtual bool scatter(const ray& r_in, const hitrecord& rec, color3& attenuation, ray& scattered) const = 0;
};

class lambertian: public material{
    public:
        lambertian(const color3& a): albedo(a) {}

        virtual bool scatter(const ray& r_in, const hitrecord& rec, color3& attenuation, ray& scattered) const override{
            auto scatter_direction = rec.normal + randomUnitVector();

            if(scatter_direction.nearZero())
                scatter_direction = rec.normal;

            scattered = ray(rec.p, scatter_direction);
            attenuation = albedo;
            return true;
        }

    public:
        color3 albedo;
};

class metal: public material {
    public:
        metal(const color3& a, double f): albedo(a), fuzz(f < 1 ? f : 1) {}

        virtual bool scatter(const ray& r_in, const hitrecord& rec, color3& attenuation, ray& scattered) const override{
            vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
            scattered = ray(rec.p, reflected + randomInUnitSphere() * fuzz);
            attenuation = albedo;
            return (dot(scattered.direction(), rec.normal) > 0);
        }

    public:
        color3 albedo;
        double fuzz;
};
