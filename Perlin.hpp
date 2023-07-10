#pragma once

#include "Utils.hpp"

class perlin{
    public:
        perlin() {
            ranvec = new vec3[pointCount];
            for(int i = 0; i < pointCount; ++i)
                ranvec[i] = unitVector(vec3::random(-1, 1));
            
            permX = perlinGeneratePerm();
            permY = perlinGeneratePerm();
            permZ = perlinGeneratePerm();
        }

        ~perlin() {
            delete[] ranvec;
            delete[] permX;
            delete[] permY;
            delete[] permZ;
        }

        
}