#ifndef VEC_3
#define VEC_3
#include<iostream>
#include<cmath>

#include "Utils.hpp"

class vec3{
    public:
        vec3() {}
        vec3(float e0, float e1, float e2) { e[0] = e0; e[1] = e1; e[2] = e2; }
        float x() const { return e[0]; }
        float y() const { return e[1]; }
        float z() const { return e[2]; }
        float r() const { return e[0]; }
        float g() const { return e[1]; }
        float b() const { return e[2]; }

        inline const vec3& operator+() const { return *this; }
        inline vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
        inline float operator[](int i) const { return e[i]; }
        inline float& operator[](int i) { return e[i]; };

        inline vec3& operator+=(const vec3 &v2){e[0] += v2.e[0];e[1] += v2.e[1]; e[2] += v2.e[2];return *this;}
        inline vec3& operator-=(const vec3 &v2){e[0] -= v2.e[0];e[1] -= v2.e[1]; e[2] -= v2.e[2];return *this;}
        inline vec3& operator*=(const vec3 &v2){e[0] *= v2.e[0];e[1] *= v2.e[1]; e[2] *= v2.e[2];return *this;}
        inline vec3& operator/=(const vec3 &v2){e[0] /= v2.e[0];e[1] /= v2.e[1]; e[2] /= v2.e[2];return *this;}
        inline vec3& operator*=(const float t){e[0] *= t;e[1] *= t; e[2] *= t;return *this;}
        inline vec3& operator/=(const float t){e[0] /= t;e[1] /= t; e[2] /= t;return *this;}

        inline float length() const {
            return sqrt(e[0]*e[0] + e[1]*e[1] + e[2]*e[2]);
        }

        inline float squared_length() const {
            return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
        }

        inline static vec3 random(){
            return vec3(randomDouble(), randomDouble(), randomDouble());
        }

        inline static vec3 random(double min, double max){
            return vec3(randomDouble(min, max), randomDouble(min, max), randomDouble(min, max));
        }

        bool nearZero() const {
            const auto s = 1e-8;
            return (fabs(e[0]) < s) && (fabs(e[1]) < s) && (fabs(e[2]) < s);
        }
        float e[3];
        
};

using point3 = vec3;
using color3 = vec3;

inline std::ostream& operator<<(std::ostream &out, const vec3 &v) {
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline double dot(const vec3 &v1, const vec3 &v2) {
    return v1.e[0]*v2.e[0] + v1.e[1]*v2.e[1] + v1.e[2]*v2.e[2]; 
}

inline vec3 cross(const vec3 &v1, const vec3 &v2) {
    return vec3(v1.e[1] * v2.e[2] - v1.e[2]*v2.e[1],
                v1.e[2] * v2.e[0] - v1.e[0]*v2.e[2],
                v1.e[0] * v2.e[1] - v1.e[1]*v2.e[0]);
}

inline vec3 operator+(const vec3 &v1, const vec3 &v2){return vec3(v1.e[0] + v2.e[0], v1.e[1] + v2.e[1], v1.e[2] + v2.e[2]);}
inline vec3 operator-(const vec3 &v1, const vec3 &v2){return vec3(v1.e[0] - v2.e[0], v1.e[1] - v2.e[1], v1.e[2] - v2.e[2]);}
inline vec3 operator*(const vec3 &v1, const vec3 &v2){return vec3(v1.e[0] * v2.e[0], v1.e[1] * v2.e[1], v1.e[2] * v2.e[2]);}
inline vec3 operator/(const vec3 &v1, const vec3 &v2){return vec3(v1.e[0] / v2.e[0], v1.e[1] / v2.e[1], v1.e[2] / v2.e[2]);}
inline vec3 operator*(const vec3 &v1, const float t){return vec3(v1.e[0] * t, v1.e[1] * t, v1.e[2] * t);}
inline vec3 operator/(const vec3 &v1, const float t){return vec3(v1.e[0] / t, v1.e[1] / t, v1.e[2] / t);}

inline vec3 unit_vector(vec3 v){
    return v / v.length();
}

vec3 randomInUnitSphere(){
    while(true){
        auto p = vec3::random(-1,1);
        if(p.squared_length() >= 1) continue;
        return p;
    }
}

vec3 randomUnitVector(){
    return unit_vector(randomInUnitSphere());
}

//计算反射向量
vec3 reflect(const vec3& v, const vec3& n){
    return v - n * 2 * dot(v,n);
}

//uv 入射光线方向，n 法线，etai_over_etat 折射率比
vec3 refract(const vec3& uv, const vec3& n, double etai_over_etat) {
    auto cos_theta = fmin(dot(-uv, n), 1.0);
    vec3 r_out_perp = (uv + n * cos_theta) * etai_over_etat;
    vec3 r_out_parallel = n * (-sqrt(fabs(1.0 - r_out_perp.length())) );
    return r_out_perp + r_out_parallel;
}

#endif