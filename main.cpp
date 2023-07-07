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
#include <random>

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<> dis(0, 1);

hitablelist random_scene() {
    hitablelist world;

    auto ground_material = make_shared<lambertian>(color3(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = randomDouble();
            point3 center(a + 0.9*randomDouble(), 0.2, b + 0.9*randomDouble());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = color3::random() * color3::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = color3::random(0.5, 1);
                    auto fuzz = randomDouble(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(color3(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(color3(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    return world;
}

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

color3 ray_color(const ray& r, const hitable& world, int depth, double pro){
    hitrecord rec;
    if(depth <= 0) return color3(0,0,0);
    //if(dis(gen) < pro)  return color3(0,0,0);

    if(world.hit(r,0.001,infinity,rec)){
        //vec3 N = unit_vector(r.at(hit_sphere(point3(0,0,-1), 0.5, r)) - vec3(0,0,-1));
        ray scattered;
        color3 attenuation;
        if(rec.matPtr->scatter(r, rec, attenuation, scattered))
            return attenuation * ray_color(scattered, world, depth - 1, pro);
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
    const int imageWidth = 1200;
    const auto aspectRatio = 3.0 / 2.0;
    const int imageHeight = static_cast<int>(imageWidth / aspectRatio);
    const int samplesPerPixel = 500;
    const int maxDepth = 100;
    const int eval = 50;
    const double pro = eval/maxDepth;

    //World
    auto world = random_scene();
    auto R = cos(pi/4);

    //Materials
    auto material_ground = make_shared<lambertian>(color3(0.8,0.8,0.0));
    auto material_center = make_shared<lambertian>(color3(0.1,0.2,0.5));
    auto material_left = make_shared<dielectric>(1.5);
    auto material_right = make_shared<metal>(color3(0.8,0.6,0.2), 1.0);

    world.add(make_shared<sphere>(point3(0.0,-100.5,-1.0), 100.0, material_ground));
    world.add(make_shared<sphere>(point3(0.0,0.0,-1.0), 0.5, material_center));
    world.add(make_shared<sphere>(point3(-1.0,0.0,-1.0), -0.4, material_left));
    world.add(make_shared<sphere>(point3(1.0,0.0,-1.0), 0.5, material_right));


    //Camera
    camera cam(point3(-2,2,1), point3(0,0,-1), vec3(0,1,0), 90.0, aspectRatio);

    std::cout << "P3\n" << imageWidth << ' ' << imageHeight << "\n255\n";
    //Render
    for(int j = imageHeight - 1; j >= 0; --j){
        //std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for(int i = 0; i < imageWidth; ++i){
            color3 pixel_color(0,0,0);
            for(int s = 0; s < samplesPerPixel; ++s){
                std::cerr << "\rScanlines remaining: " << j << ' ' << "Scanheight remaining: " << i << ' ' << "Samples remaining: " << s << ' ' << std::flush;
                auto u = double(i + randomDouble()) / (imageWidth - 1);
                auto v = double(j + randomDouble()) / (imageHeight - 1);
                ray r = cam.getRay(u,v);
                pixel_color += ray_color(r,world, maxDepth,pro);
            }
            write_color(std::cout, pixel_color, samplesPerPixel);
        }
    }
    return 0;
}