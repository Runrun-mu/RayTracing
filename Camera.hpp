#pragma once

#include "Utils.hpp"

class camera {
    public:
        camera(point3 lookfrom, point3 lookat, vec3 vup, double vfov, double aspectRatio
            , double aperture, double focusDist, double _time0, double _time1){
            auto theta = degree2Radians(vfov);
            auto h = tan(theta/2);
            auto viewportHeight = 2.0 * h;
            auto viewportWidth = aspectRatio * viewportHeight;

            w = unit_vector(lookfrom - lookat);
            u = unit_vector(cross(vup, w));
            v = cross(w, u);
            auto focalLength = 1.0;

            origin = lookfrom;
            horizontal = u * focusDist * viewportWidth;
            vertical = v * focusDist * viewportHeight;
            lowerLeftCorner = origin - horizontal/2 - vertical/2 - w * focusDist;//左下角在空间中的位置

            lensRadius = aperture / 2;
            time0 = _time0;
            time1 = _time1;
        }

        ray getRay(double v1, double v2) const {
            vec3 rd = randomInUnitDisk() * lensRadius;
            vec3 offset = u * rd.x() + v * rd.y();

            return ray(origin + offset, lowerLeftCorner + horizontal * v1 + vertical * v2 - origin - offset, randomDouble(time0, time1));
        }
    private:
        point3 origin;
        point3 lowerLeftCorner;
        vec3 horizontal;
        vec3 vertical;
        vec3 u, v, w;
        double lensRadius;
        double time0, time1;
};