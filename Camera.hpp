#pragma once

#include "Utils.hpp"

class camera {
    public:
        camera(){
            auto aspectRatio = 16.0 / 9.0;
            auto viewportHeight = 2.0;
            auto viewportWidth = aspectRatio * viewportHeight;
            auto focalLength = 1.0;

            origin = point3(0, 0, 0);
            horizontal = vec3(viewportWidth, 0, 0);
            vertical = vec3(0, viewportHeight, 0);
            lowerLeftCorner = origin - horizontal/2 - vertical/2 - vec3(0, 0, focalLength);
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