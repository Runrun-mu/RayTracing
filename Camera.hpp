#pragma once

#include "Utils.hpp"

class camera {
    public:
        camera(point3 lookfrom, point3 lookat, vec3 vup, double vfov, double aspectRatio){
            auto theta = degree2Radians(vfov);
            auto h = tan(theta/2);
            auto viewportHeight = 2.0 * h;
            auto viewportWidth = aspectRatio * viewportHeight;

            auto w = unit_vector(lookfrom - lookat);
            auto u = unit_vector(cross(vup, w));
            auto v = cross(w, u);
            auto focalLength = 1.0;

            origin = lookfrom;
            horizontal = u * viewportWidth;
            vertical = v * viewportHeight;
            lowerLeftCorner = origin - horizontal/2 - vertical/2 - w;//左下角在空间中的位置
        }

        ray getRay(double v1, double v2) const {
            return ray(origin, lowerLeftCorner + horizontal * v1 + vertical * v2 - origin);
        }
    private:
        point3 origin;
        point3 lowerLeftCorner;
        vec3 horizontal;
        vec3 vertical;
};