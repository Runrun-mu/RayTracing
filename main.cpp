#include <iostream>
#include "Vector.hpp"
#include "Color.hpp"
#include "Ray.hpp"
#include "Hitablelist.hpp"
#include "Utils.hpp"
#include "Sphere.hpp"
#include "Camera.hpp"
#include "Hitable.hpp"
#include "Material.hpp"

double hit_sphere(const point3& center, double radius, const ray& r){
    vec3 oc = r.origin() - center;
    auto a = dot(r.direction(), r.direction());
    auto b = dot(oc, r.direction()) * 2;
    auto c = dot(oc,oc) - radius * radius;
    auto discriminant = b*b - 4*a*c;
    if(discriminant < 0)
        return -1.0;
    else
        return (- b - sqrt(discriminant)) / (2.0 * a);
}

color3 ray_color(const ray& r, const hitable& world, int depth){
    hitrecord rec;
    if(depth <= 0) return color3(0,0,0);

    if(world.hit(r,0.001,infinity,rec)){
        //vec3 N = unit_vector(r.at(hit_sphere(point3(0,0,-1), 0.5, r)) - vec3(0,0,-1));
        ray scattered;
        color3 attenuation;
        if(rec.matPtr->scatter(r, rec, attenuation, scattered))
            return attenuation * ray_color(scattered, world, depth - 1);
        return color3(0,0,0);
        //point3 target = rec.p + rec.normal + randomUnitVector();
        //return  (ray_color(ray(rec.p, target - rec.p), world, depth - 1)) * 0.5;
    }
    vec3 unitDirection = unit_vector(r.direction());
    auto t = 0.5*(unitDirection.y() + 1.0);
    return color3(1.0,1.0,1.0) * (1 - t) + color3(0.5,0.7,1.0) * t;
}

int main(){

    //Image
    const int imageWidth = 400;
    const auto aspectRatio = 16.0 / 9.0;
    const int imageHeight = static_cast<int>(imageWidth / aspectRatio);
    const int samplesPerPixel = 1000;
    const int maxDepth = 100;

    //World
    hitablelist world;

    //Materials
    auto material_ground = make_shared<lambertian>(color3(0.8,0.8,0.0));
    auto material_center = make_shared<lambertian>(color3(0.1,0.2,0.5));
    auto material_left = make_shared<metal>(color3(0.8,0.8,0.8), 0.3);
    auto material_right = make_shared<metal>(color3(0.8,0.6,0.2), 1.0);

    world.add(make_shared<sphere>(point3(0.0,-100.5,-1.0), 100.0, material_ground));
    world.add(make_shared<sphere>(point3(0.0,0.0,-1.0), 0.5, material_center));
    world.add(make_shared<sphere>(point3(-1.0,0.0,-1.0), 0.5, material_left));
    world.add(make_shared<sphere>(point3(1.0,0.0,-1.0), 0.5, material_right));


    //Camera
    camera cam;

    std::cout << "P3\n" << imageWidth << ' ' << imageHeight << "\n255\n";
    //Render
    for(int j = imageHeight - 1; j >= 0; --j){
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for(int i = 0; i < imageWidth; ++i){
            color3 pixel_color(0,0,0);
            for(int s = 0; s < samplesPerPixel; ++s){
                auto u = double(i + randomDouble()) / (imageWidth - 1);
                auto v = double(j + randomDouble()) / (imageHeight - 1);
                ray r = cam.getRay(u,v);
                pixel_color += ray_color(r,world, maxDepth);
            }
            write_color(std::cout, pixel_color, samplesPerPixel);
        }
    }
    return 0;
}