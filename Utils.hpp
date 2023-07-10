#pragma once

#include <cmath>
#include <limits>
#include <memory>
#include <random>

// Usings
using std::shared_ptr;
using std::make_shared;
using std::sqrt;

// Constants
const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utility Functions
inline double degree2Radians(double degrees){
    return degrees * pi / 180.0;
}

inline double randomDouble(){
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}

inline double randomDouble(double min, double max){
    return min + (max-min) * randomDouble();
}

inline double clamp(double x, double min, double max){
    if(x < min) return min;
    if(x > max) return max;
    return x;
}

inline int randomInt(int min, int max){
    return static_cast<int>(randomDouble(min, max+1));
}
