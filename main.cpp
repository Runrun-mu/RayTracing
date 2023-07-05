#include <iostream>
#include "Vector.hpp"
#include "Color.hpp"
#include "Ray.hpp"

bool hit_sphere(const point3& center, double radius, const ray& r){
    vec3 oc = r.origin() - center;
    auto a = dot(r.direction(), r.direction());
    auto b = dot(oc, r.direction()) * 2;
    auto c = dot(oc,oc) - radius * radius;
    auto discriminant = b*b - 4*a*c;
    return (discriminant > 0);
}

color3 ray_color(const ray& r){
    if(hit_sphere(point3(0,0,-1), 0.5, r))
        return color3(1,0,0);
    vec3 unitDirection = unit_vector(r.direction());
    auto t = 0.5*(unitDirection.y() + 1.0);
    return color3(1.0,1.0,1.0) * (1 - t) + color3(0.5,0.7,1.0) * t;
}

int main(){

    //Image
    const int imageWidth = 400;
    const auto aspectRatio = 16.0 / 9.0;
    const int imageHeight = static_cast<int>(imageWidth / aspectRatio);

    //Camera
    auto viewPortHeight = 2.0;
    auto viewPortWidth = viewPortHeight * aspectRatio;
    auto focalLength = 1.0;

    auto origin = point3(0,0,0);
    auto horizontal = vec3(viewPortWidth, 0, 0);
    auto vertical = vec3(0, viewPortHeight, 0);
    auto lowerLeftCorner = origin - horizontal / 2 - vertical / 2 - vec3(0,0,focalLength);

    std::cout << "P3\n" << imageWidth << ' ' << imageHeight << "\n255\n";
    //Render
    for(int j = imageHeight - 1; j >= 0; --j){
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for(int i = 0; i < imageWidth; ++i){
            auto u = double(i) / (imageWidth - 1);
            auto v = double(j) / (imageHeight - 1);
            ray r(origin, lowerLeftCorner + horizontal * u + vertical * v - origin);
            color3 pixel_color = ray_color(r);
            write_color(std::cout, pixel_color);
        }
    }
    return 0;
}