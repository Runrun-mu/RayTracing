#pragma once

#include "Utils.hpp"
#include "Ray.hpp"

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

class dielectric : public material {
    public:
        dielectric(double index_of_refraction) : ir(index_of_refraction) {}

        virtual bool scatter(
            const ray& r_in, const hitrecord& rec, color3& attenuation, ray& scattered
        ) const override {
            attenuation = color3(1.0, 1.0, 1.0);
            double refraction_ratio = rec.frontFace ? (1.0/ir) : ir;

            vec3 unit_direction = unit_vector(r_in.direction());
            double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
            double sin_theta = sqrt(1.0 - cos_theta*cos_theta);
            bool cannot_refract = refraction_ratio * sin_theta > 1.0;
            vec3 direction;
            if(cannot_refract || reflectance(cos_theta, refraction_ratio) > randomDouble()){
                direction = reflect(unit_direction, rec.normal);
            }
            else{
                direction = refract(unit_direction, rec.normal, refraction_ratio);
            }

            scattered = ray(rec.p, direction);
            return true;
        }

    public:
        double ir; // Index of Refraction
    
    private:
        static double reflectance(double cosine, double ref_idx) {
            auto r0 = (1-ref_idx) / (1+ref_idx);
            r0 = r0 * r0;
            return r0 + (1-r0)*pow((1 - cosine),5);
        }
};