#pragma once

#include "Utils.hpp"
#include "Ray.hpp"
#include "Texture.hpp"

struct hitrecord;

class material{
    public:
        virtual color3 emitted(double u, double v, const point3& p) const{
            return color3(0, 0, 0);
        }
        virtual bool scatter(const ray& r_in, const hitrecord& rec, color3& attenuation, ray& scattered, double& pdf) const {
            return false;
        }
        virtual bool scatter(const ray& r_in, const hitrecord& rec, color3& attenuation, ray& scattered) const {
            return false;
        }
        virtual double scattering_pdf(const ray& r_in, const hitrecord& rec, const ray& scattered) const{
            return 0;
        }

};

class lambertian: public material{
    public:
        lambertian(const color3& a): albedo(make_shared<solidcolor>(a)) {}
        lambertian(shared_ptr<texture> a): albedo(a) {}


        virtual bool scatter(const ray& r_in, const hitrecord& rec, color3& attenuation, ray& scattered, double& pdf) const override{
            auto scatter_direction = rec.normal + randomUnitVector();
            auto direction = randomInHemisphere(rec.normal);
            if(scatter_direction.nearZero())
                scatter_direction = rec.normal;
    
            scattered = ray(rec.p, unit_vector(direction),r_in.time());
            attenuation = albedo->value(rec.u, rec.v, rec.p);
            pdf = 0.5 / pi;
            return true;
        }

        double scattering_pdf(const ray& r_in, const hitrecord& rec, const ray& scattered) const override{
            auto cosine = dot(rec.normal, unit_vector(scattered.direction()));
            return cosine < 0 ? 0 : cosine / pi;
        }

    public:
        shared_ptr<texture> albedo;
};

class metal: public material {
    public:
        metal(const color3& a, double f): albedo(a), fuzz(f < 1 ? f : 1) {}

        virtual bool scatter(const ray& r_in, const hitrecord& rec, color3& attenuation, ray& scattered) const override{
            vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
            scattered = ray(rec.p, reflected + randomInUnitSphere() * fuzz, r_in.time());
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

            scattered = ray(rec.p, direction,r_in.time());
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

class diffuselight : public material {
    public:
        diffuselight(shared_ptr<texture> a) : emit(a) {}
        diffuselight(color3 c) : emit(make_shared<solidcolor>(c)) {}

        virtual bool scatter(const ray& r_in, const hitrecord& rec, color3& attenuation, ray& scattered) const override{
            return false;
        }

        virtual color3 emitted(double u, double v, const point3& p) const override{
            return emit->value(u, v, p);
        }

    public:
        shared_ptr<texture> emit;
};