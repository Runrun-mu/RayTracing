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
#include "Rect.hpp"
#include "Box.hpp"
#include "BVH.hpp"
#include <omp.h>
#include <random>

using std::shared_ptr;
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<> dis(0, 1);

hitablelist random_scene() {
    hitablelist world;
    hitablelist boxlist;

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
                    auto center2 = center + vec3(0, randomDouble(0,.5), 0);
                    boxlist.add(make_shared<moveing_sphere>(center, center2, 0.0, 1.0, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = color3::random(0.5, 1);
                    auto fuzz = randomDouble(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    boxlist.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    boxlist.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    world.add(make_shared<bvhnode>(boxlist, 0.0, 1.0));

    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(color3(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(color3(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    return world;
}

hitablelist two_spheres() {
    hitablelist objects;

    auto checker = make_shared<checkertexture>(color3(0.2, 0.3, 0.1), color3(0.9, 0.9, 0.9));

    objects.add(make_shared<sphere>(point3(0,-10, 0), 10, make_shared<lambertian>(checker)));
    objects.add(make_shared<sphere>(point3(0, 10, 0), 10, make_shared<lambertian>(checker)));

    return objects;
}

hitablelist two_checker_spheres() {
    hitablelist objects;

    auto pertext = make_shared<checkertexture>(color3(0.2, 0.3, 0.1), color3(0.9, 0.9, 0.9));
    objects.add(make_shared<sphere>(point3(0,-1000,0), 1000, make_shared<lambertian>(pertext)));
    objects.add(make_shared<sphere>(point3(0, 2, 0), 2, make_shared<lambertian>(pertext)));

    return objects;
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

color3 ray_color(const ray& r, const color3& background, const hitable& world, int depth, double pro){
    hitrecord rec;
    if(depth <= 0) return color3(0,0,0);
    //if(dis(gen) < pro)  return color3(0,0,0);

    if(!world.hit(r, 0.001,infinity,rec)) return background;
        //vec3 N = unit_vector(r.at(hit_sphere(point3(0,0,-1), 0.5, r)) - vec3(0,0,-1));
    ray scattered;
    color3 attenuation;
    color3 emitted = rec.matPtr->emitted(rec.u, rec.v, rec.p);

    if(!rec.matPtr->scatter(r, rec, attenuation, scattered))
        return emitted;
    return emitted + attenuation * ray_color(scattered, background, world, depth - 1, pro);
        //point3 target = rec.p + rec.normal + randomUnitVector();
        //return  (ray_color(ray(rec.p, target - rec.p), world, depth - 1)) * 0.5;
    //vec3 unitDirection = unit_vector(r.direction());
    //auto t = 0.5*(unitDirection.y() + 1.0);
    //return color3(1.0,1.0,1.0) * (1 - t) + color3(0.5,0.7,1.0) * t;
}

hitablelist simple_light(){
    hitablelist objects;

    auto pertext = make_shared<checkertexture>(color3(0.2, 0.3, 0.1), color3(0.9, 0.9, 0.9));
    objects.add(make_shared<sphere>(point3(0,-1000,0), 1000, make_shared<lambertian>(pertext)));
    objects.add(make_shared<sphere>(point3(0, 2, 0), 2, make_shared<lambertian>(pertext)));

    auto difflight = make_shared<diffuselight>(color3(4,4,4));
    objects.add(make_shared<xyrect>(3,5,1,3,-2,difflight));

    return objects;
}

hitablelist conell_box(){
    hitablelist objects;

    auto red = make_shared<lambertian>(color3(0.65, 0.05, 0.05));
    auto white = make_shared<lambertian>(color3(0.73, 0.73, 0.73));
    auto green = make_shared<lambertian>(color3(0.12, 0.45, 0.15));
    auto light = make_shared<diffuselight>(color3(15, 15, 15));

    objects.add(make_shared<yzrect>(0, 555, 0, 555, 555, green));
    objects.add(make_shared<yzrect>(0, 555, 0, 555, 0, red));
    objects.add(make_shared<xzrect>(213, 343, 227, 332, 554, light));
    objects.add(make_shared<xzrect>(0, 555, 0, 555, 0, white));
    objects.add(make_shared<xzrect>(0, 555, 0, 555, 555, white));
    objects.add(make_shared<xyrect>(0, 555, 0, 555, 555, white));

    shared_ptr<hitable> box1 = make_shared<box>(point3(0, 0, 0), point3(165, 330, 165), white);
    box1 = make_shared<rotatey>(box1, 15);
    box1 = make_shared<translate>(box1, vec3(265, 0, 295));
    objects.add(box1);
    
    shared_ptr<hitable> box2 = make_shared<box>(point3(0, 0, 0), point3(165, 165, 165), white);
    box2 = make_shared<rotatey>(box2, -18);
    box2 = make_shared<translate>(box2, vec3(130, 0, 65));
    objects.add(box2);

    return objects;
}

int main(){

    //Image
    int imageWidth = 400;
    auto aspectRatio = 16.0 / 9.0;
    int samplesPerPixel = 100;
    const int maxDepth = 100;
    const int eval = 50;
    const double pro = eval/maxDepth;

    //World
    auto world = random_scene();
    auto R = cos(pi/4);

    //Materials
    auto material_ground = make_shared<checkertexture>(color3(0.2,0.3,0.1), color3(0.9,0.9,0.9));
    //auto material_center = make_shared<lambertian>(color3(0.1,0.2,0.5));
    //auto material_left = make_shared<dielectric>(1.5);
    //auto material_right = make_shared<metal>(color3(0.8,0.6,0.2), 1.0);

    world.add(make_shared<sphere>(point3(0.0,-1000,0), 1000.0, make_shared<lambertian>(material_ground)));
    //world.add(make_shared<sphere>(point3(0.0,0.0,-1.0), 0.5, material_center));
    //world.add(make_shared<sphere>(point3(-1.0,0.0,-1.0), -0.4, material_left));
    //world.add(make_shared<sphere>(point3(1.0,0.0,-1.0), 0.5, material_right));
    int mode = 0;
    std::cerr << " Choose one scene " << std::endl;
    std::cin >> mode;
    //Camera
    point3 lookfrom;
    point3 lookat;
    auto vfov = 40.0;
    auto aperture = 0.0;
    vec3 vup(0,1,0);
    auto dis2Focus = 10.0;
    color3 background = color3(0,0,0);
    switch(mode){
        case 1:
            world = random_scene();
            lookfrom = point3(13,2,3);
            lookat = point3(0,0,0);
            vfov = 20.0;
            aperture = 0.1;
            imageWidth = 800;
            background = color3(0.70, 0.80, 1.00);
            break;
        case 2:
            world = two_spheres();
            lookfrom = point3(13,2,3);
            lookat = point3(0,0,0);
            background = color3(0.70, 0.80, 1.00);
            vfov = 20.0;
            break;
        case 3:
            world = two_checker_spheres();
            lookfrom = point3(13,2,3);
            lookat = point3(0,0,0);
            background = color3(0.70, 0.80, 1.00);
            vfov = 20.0;
            break;
        case 4:
            world = simple_light();
            lookfrom = point3(26,3,6);
            lookat = point3(0,2,0);
            background = color3(0,0,0);
            samplesPerPixel = 400;
            vfov = 20.0;
            break;
        case 5:
            world = conell_box();
            lookfrom = point3(278, 278, -800);
            lookat = point3(278, 278, 0);
            background = color3(0,0,0);
            samplesPerPixel = 200;
            aspectRatio = 1.0;
            imageWidth = 600;
            vfov = 40.0;
            break;
    }
    const int imageHeight = static_cast<int>(imageWidth / aspectRatio);
    camera cam(lookfrom, lookat, vup, vfov, aspectRatio, aperture, dis2Focus, 0.0, 1.0);

    std::cout << "P3\n" << imageWidth << ' ' << imageHeight << "\n255\n";
    //Render
    //std::cout << imageHeight << std::endl;
    #pragma omp parallel for
    for(int j = imageHeight - 1; j >= 0; --j){
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for(int i = 0; i < imageWidth; ++i){
            color3 pixel_color(0,0,0);
            for(int s = 0; s < samplesPerPixel; ++s){
                //std::cerr << "\rScanlines remaining: " << j << ' ' << "Scanheight remaining: " << i << ' ' << "Samples remaining: " << s << ' ' << std::flush;
                auto u = double(i + randomDouble()) / (imageWidth - 1);
                auto v = double(j + randomDouble()) / (imageHeight - 1);
                ray r = cam.getRay(u,v);
                pixel_color += ray_color(r ,background, world, maxDepth,pro);
            }
            write_color(std::cout, pixel_color, samplesPerPixel);
        }
    }
    return 0;
}