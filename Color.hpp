#pragma once
#include "Vector.hpp"
#include "Utils.hpp"

void write_color(std::ostream &out, color3 pixel_color, int samplesPerPixel) {
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    auto scale = 1.0 / samplesPerPixel;

    r *= scale;
    g *= scale;
    b *= scale;
    
    out << static_cast<int>(255.999 * clamp(r, 0.0, 0.999)) << ' '
        << static_cast<int>(255.999 * clamp(g, 0.0, 0.999)) << ' '
        << static_cast<int>(255.999 * clamp(b, 0.0, 0.999)) << '\n';
}
