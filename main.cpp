#include <iostream>
#include "Vector.hpp"
#include "Color.hpp"
#include "Ray.hpp"
#include "Hitablelist.hpp"
#include "Utils.hpp"
#include "Sphere.hpp"
#include "Camera.hpp"
#include "Hitable.hpp"

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

    if(world.hit(r,0,infinity,rec)){
        //vec3 N = unit_vector(r.at(hit_sphere(point3(0,0,-1), 0.5, r)) - vec3(0,0,-1));
        point3 target = rec.p + rec.normal + randomInUnitSphere();
        return  (ray_color(ray(rec.p, target - rec.p), world, depth - 1)) * 0.5;
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
    const int samplesPerPixel = 100;
    const int maxDepth = 50;
    //World
    hitablelist world;
    world.add(make_shared<sphere>(point3(0,0,-1), 0.5));
    world.add(make_shared<sphere>(point3(0,-100.5,-1), 100));

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