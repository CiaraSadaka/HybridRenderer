#pragma once

#include "pch.h"
#include <cmath>
#include <iostream>
//#include "CpuWriteToTextureDemo.h"
#include "DirectXTK/SimpleMath.h"
#include "DirectXMath.h"
//#include "GameException.h"
//#include <exception>
#include <stdexcept>



//#include "RayHelper.h"

using std::sqrt;
using std::fabs;

using namespace std;
using namespace DirectX;
//using namespace Library;
//using namespace Rendering;
//using namespace Rendering::Hybrid;

class vec3 
{
public:
    vec3() : _vec{ 0, 0, 0 } {}
    vec3(float e0, float e1, float e2) : _vec{ e0, e1, e2 } {}

    //vec3(float e0, float e1, float e2)
    //{
    ////    _vec;
    ////    DirectX::XMVectorSetIntX(_vec, static_cast<uint32_t>(e0));

    ////    DirectX::XMVectorSetIntY(_vec, static_cast<uint32_t>(e1));

    ////    DirectX::XMVectorSetIntZ(_vec, static_cast<uint32_t>(e2));
    //      _vec;
    //   /* DirectX::XMFLOAT3 a(e0, e1, e2);
    //    XMLoadFloat3(&a);*/
    //    _vec = DirectX::XMVectorSet(e0, e1, e2, 1.0f);
    //    _vec;
    //}

    float x() const { return _vec.x; }
    float y() const { return _vec.y; }
    float z() const { return  _vec.z; }

    vec3 operator-() const { return vec3(-x(), -y(), -z()); }
    float operator[](int i) const { 
        if (i == 0)
            return _vec.x;
        else if (i == 1)
            return _vec.y;
        else if (i == 2)
            return _vec.z;
        else
            throw std::invalid_argument("Trying to access invalid float3 index");
    }
    float& operator[](int i) { 
        if (i == 0)
            return _vec.x;
        else if (i == 1)
            return _vec.y;
        else if (i == 2)
            return _vec.z;
        else
            throw std::invalid_argument("Trying to access invalid float3 index");
    }//possible error

    vec3& operator+=(const vec3& v) {
        _vec.x += v.x();
        _vec.y += v.y();
        _vec.z += v.z();
        return *this;
    }

    vec3& operator*=(const float t) {

        _vec.x *= t;
        _vec.y *= t;
        _vec.z *= t;

        return *this;
    }

    vec3& operator/=(const float t) {
        return *this *= 1 / t;
    }

    float length() const {
        return sqrt(length_squared());
    }

    float length_squared() const {
        return static_cast<float>( _vec.x * _vec.x + _vec.y * _vec.y + _vec.z * _vec.z);
    }

    bool near_zero() const {
        // Return true if the vector is close to zero in all dimensions.
        const auto s = 1e-8;
        float x = _vec.x;
        float y = _vec.y;
        float z = _vec.z;

        return (fabs(x) < s) && (fabs(y) < s) && (fabs(z) < s);
    }

    inline static vec3 random() {
        return vec3(random_float(), random_float(), random_float());
    }

    inline static vec3 random(float min, float max) {
        return vec3(random_float(min, max), random_float(min, max), random_float(min, max));
    }

public:
    DirectX::XMFLOAT3 _vec;
};


// Type aliases for vec3
//using vec3::DirectX
using point3 = vec3;   // 3D point
using color = vec3;    // RGB color


// vec3 Utility Functions

inline std::ostream& operator<<(std::ostream& out, const vec3& v) {
    float x = v.x();
    float y = v.y();
    float z = v.z();
    return out << x << ' ' << y << ' ' << z;
}

inline vec3 operator+(const vec3& u, const vec3& v) {

    float v_x = v.x();
    float v_y = v.y();
    float v_z = v.z();

    float u_x = u.x();
    float u_y = u.y();
    float u_z = u.z();

    return vec3(u_x + v_x, u_y + v_y, u_z + v_z);
}

inline vec3 operator-(const vec3& u, const vec3& v) {
    float v_x = v.x();
    float v_y = v.y();
    float v_z = v.z();

    float u_x = u.x();
    float u_y = u.y();
    float u_z = u.z();
    return vec3(u_x - v_x, u_y - v_y, u_z - v_z);
}

inline vec3 operator*(const vec3& u, const vec3& v) {
    float v_x = v.x();
    float v_y = v.y();
    float v_z = v.z();

    float u_x = u.x();
    float u_y = u.y();
    float u_z = u.z();

    return vec3(u_x * v_x, u_y * v_y, u_z * v_z);
}

inline vec3 operator*(float t, const vec3& v) {
    float x = v.x();
    float y = v.y();
    float z = v.z();


    return vec3(t * x, t * y, t * z);
}

inline vec3 operator*(const vec3& v, float t) {
    return t * v;
}

inline vec3 operator/(vec3 v, float t) {
    return (1 / t) * v;
}

inline float dot(const vec3& u, const vec3& v) {

    float v_x = v.x();
    float v_y = v.y();
    float v_z = v.z();

    float u_x = u.x();
    float u_y = u.y();
    float u_z = u.z();

    return u_x * v_x 
        + u_y * v_y
        + u_z * v_z;
}

inline vec3 cross(const vec3& u, const vec3& v) {
    float v_x = v.x();
    float v_y = v.y();
    float v_z = v.z();

    float u_x = u.x();
    float u_y = u.y();
    float u_z = u.z();


    return vec3(u_y * v_z - u_z * v_y,
        u_z * v_x - u_x * v_z,
        u_x * v_y - u_y * v_x);
}

inline vec3 unit_vector(vec3 v) {
    return v / v.length();
}

//inline vec3 random_in_unit_disk() {
//    while (true) {
//        auto p = vec3(random_float(-1, 1), random_float(-1, 1), 0);
//        if (p.length_squared() >= 1) continue;
//        return p;
//    }
//}

inline vec3 random_in_unit_sphere() {
    while (true) {
        auto p = vec3::random(-1, 1);
        if (p.length_squared() >= 1) continue;
        return p;
    }
}

inline vec3 random_unit_vector() {
    return unit_vector(random_in_unit_sphere());
}

inline vec3 random_in_hemisphere(const vec3& normal) {
    vec3 in_unit_sphere = random_in_unit_sphere();
    if (dot(in_unit_sphere, normal) > 0.0) // In the same hemisphere as the normal
        return in_unit_sphere;
    else
        return -in_unit_sphere;
}

inline vec3 reflect(const vec3& v, const vec3& n) {
    return v - 2 * dot(v, n) * n;
}

inline vec3 refract(const vec3& uv, const vec3& n, float etai_over_etat) {
    auto cos_theta = fmin(dot(-uv, n), 1.0f);
    vec3 r_out_perp = etai_over_etat * (uv + cos_theta * n);
    vec3 r_out_parallel = -sqrt(fabs(1.0f - r_out_perp.length_squared())) * n;
    return r_out_perp + r_out_parallel;
}















//class vec3 
//{
//public:
//    vec3() : _vec{ 0, 0, 0 } {}
//   // vec3(float e0, float e1, float e2) : _vec{ e0, e1, e2 } {}
//
//    vec3(float e0, float e1, float e2)
//    {
//        DirectX::XMVectorSetIntX(_vec, static_cast<uint32_t>(e0));
//
//        DirectX::XMVectorSetIntY(_vec, static_cast<uint32_t>(e1));
//
//        DirectX::XMVectorSetIntZ(_vec, static_cast<uint32_t>(e2));
//    }
//
//    float x() const { return static_cast<float>(DirectX::XMVectorGetIntX(_vec)); }
//    float y() const { return static_cast<float>(DirectX::XMVectorGetIntY(_vec)); }
//    float z() const { return static_cast<float>(DirectX::XMVectorGetIntZ(_vec)); }
//
//    vec3 operator-() const { return vec3(-x(), -y(), -z()); }
//    float operator[](int i) const { return DirectX::XMVectorGetByIndex(_vec, i); }
//    float& operator[](int i) { 
//        float f;
//        DirectX::XMVectorGetByIndexPtr(&f, _vec, i);
//        return f; 
//    }//possible error
//
//    vec3& operator+=(const vec3& v) {
//        float x = static_cast<float>(DirectX::XMVectorGetIntX(_vec));
//        x += v.x();
//        DirectX::XMVectorSetIntX(_vec, static_cast<uint32_t>(x));
//
//        float y = static_cast<float>(DirectX::XMVectorGetIntY(_vec));
//        y += v.y();
//        DirectX::XMVectorSetIntY(_vec, static_cast<uint32_t>(y));
//
//        float z = static_cast<float>(DirectX::XMVectorGetIntZ(_vec));
//        z += v.z();
//        DirectX::XMVectorSetIntZ(_vec, static_cast<uint32_t>(z));
//        
//        return *this;
//    }
//
//    vec3& operator*=(const float t) {
//
//        float x = static_cast<float>(DirectX::XMVectorGetIntX(_vec));
//        x*= t;
//        DirectX::XMVectorSetIntX(_vec, static_cast<uint32_t>(x));
//
//        float y = static_cast<float>(DirectX::XMVectorGetIntY(_vec));
//        y *= t;
//        DirectX::XMVectorSetIntY(_vec, static_cast<uint32_t>(y));
//
//        float z = static_cast<float>(DirectX::XMVectorGetIntZ(_vec));
//        z *= t;
//        DirectX::XMVectorSetIntZ(_vec, static_cast<uint32_t>(z));
//
//        return *this;
//    }
//
//    vec3& operator/=(const float t) {
//        return *this *= 1 / t;
//    }
//
//    float length() const {
//        return sqrt(length_squared());
//    }
//
//    float length_squared() const {
//        return static_cast<float>(DirectX::XMVectorGetIntX(_vec) * DirectX::XMVectorGetIntX(_vec) + DirectX::XMVectorGetIntY(_vec) * DirectX::XMVectorGetIntY(_vec) + DirectX::XMVectorGetIntZ(_vec) * DirectX::XMVectorGetIntZ(_vec));
//    }
//
//    bool near_zero() const {
//        // Return true if the vector is close to zero in all dimensions.
//        const auto s = 1e-8;
//        float x = static_cast<float>(DirectX::XMVectorGetIntX(_vec));
//        float y = static_cast<float>(DirectX::XMVectorGetIntY(_vec));
//        float z = static_cast<float>(DirectX::XMVectorGetIntZ(_vec));
//
//        return (fabs(x) < s) && (fabs(y) < s) && (fabs(z) < s);
//    }
//
//    inline static vec3 random() {
//        return vec3(random_float(), random_float(), random_float());
//    }
//
//    inline static vec3 random(float min, float max) {
//        return vec3(random_float(min, max), random_float(min, max), random_float(min, max));
//    }
//
//public:
//    DirectX::XMVECTOR _vec;
//};
//
//
//// Type aliases for vec3
////using vec3::DirectX
//using point3 = vec3;   // 3D point
//using color = vec3;    // RGB color
//
//
//// vec3 Utility Functions
//
//inline std::ostream& operator<<(std::ostream& out, const vec3& v) {
//   
//    float x = static_cast<float>(DirectX::XMVectorGetIntX(v._vec));
//    float y = static_cast<float>(DirectX::XMVectorGetIntY(v._vec));
//    float z = static_cast<float>(DirectX::XMVectorGetIntZ(v._vec));
//    return out << x << ' ' << y << ' ' << z;
//}
//
//inline vec3 operator+(const vec3& u, const vec3& v) {
//
//    float v_x = static_cast<float>(DirectX::XMVectorGetIntX(v._vec));
//    float v_y = static_cast<float>(DirectX::XMVectorGetIntY(v._vec));
//    float v_z = static_cast<float>(DirectX::XMVectorGetIntZ(v._vec));
//
//    float u_x = static_cast<float>(DirectX::XMVectorGetIntX(u._vec));
//    float u_y = static_cast<float>(DirectX::XMVectorGetIntY(u._vec));
//    float u_z = static_cast<float>(DirectX::XMVectorGetIntZ(u._vec));
//
//    return vec3(u_x + v_x, u_y + v_y, u_z + v_z);
//}
//
//inline vec3 operator-(const vec3& u, const vec3& v) {
//    float v_x = static_cast<float>(DirectX::XMVectorGetIntX(v._vec));
//    float v_y = static_cast<float>(DirectX::XMVectorGetIntY(v._vec));
//    float v_z = static_cast<float>(DirectX::XMVectorGetIntZ(v._vec));
//
//    float u_x = static_cast<float>(DirectX::XMVectorGetIntX(u._vec));
//    float u_y = static_cast<float>(DirectX::XMVectorGetIntY(u._vec));
//    float u_z = static_cast<float>(DirectX::XMVectorGetIntZ(u._vec));
//    return vec3(u_x - v_x, u_y - v_y, u_z - v_z);
//}
//
//inline vec3 operator*(const vec3& u, const vec3& v) {
//    float v_x = static_cast<float>(DirectX::XMVectorGetIntX(v._vec));
//    float v_y = static_cast<float>(DirectX::XMVectorGetIntY(v._vec));
//    float v_z = static_cast<float>(DirectX::XMVectorGetIntZ(v._vec));
//
//    float u_x = static_cast<float>(DirectX::XMVectorGetIntX(u._vec));
//    float u_y = static_cast<float>(DirectX::XMVectorGetIntY(u._vec));
//    float u_z = static_cast<float>(DirectX::XMVectorGetIntZ(u._vec));
//
//    return vec3(u_x * v_x, u_y * v_y, u_z * v_z);
//}
//
//inline vec3 operator*(float t, const vec3& v) {
//    float x = static_cast<float>(DirectX::XMVectorGetIntX(v._vec));
//    float y = static_cast<float>(DirectX::XMVectorGetIntY(v._vec));
//    float z = static_cast<float>(DirectX::XMVectorGetIntZ(v._vec));
//
//
//    return vec3(t * x, t * y, t * z);
//}
//
//inline vec3 operator*(const vec3& v, float t) {
//    return t * v;
//}
//
//inline vec3 operator/(vec3 v, float t) {
//    return (1 / t) * v;
//}
//
//inline float dot(const vec3& u, const vec3& v) {
//
//    float v_x = static_cast<float>(DirectX::XMVectorGetIntX(v._vec));
//    float v_y = static_cast<float>(DirectX::XMVectorGetIntY(v._vec));
//    float v_z = static_cast<float>(DirectX::XMVectorGetIntZ(v._vec));
//
//    float u_x = static_cast<float>(DirectX::XMVectorGetIntX(u._vec));
//    float u_y = static_cast<float>(DirectX::XMVectorGetIntY(u._vec));
//    float u_z = static_cast<float>(DirectX::XMVectorGetIntZ(u._vec));
//
//    return u_x * v_x 
//        + u_y * v_y
//        + u_z * v_z;
//}
//
//inline vec3 cross(const vec3& u, const vec3& v) {
//    float v_x = static_cast<float>(DirectX::XMVectorGetIntX(v._vec));
//    float v_y = static_cast<float>(DirectX::XMVectorGetIntY(v._vec));
//    float v_z = static_cast<float>(DirectX::XMVectorGetIntZ(v._vec));
//
//    float u_x = static_cast<float>(DirectX::XMVectorGetIntX(u._vec));
//    float u_y = static_cast<float>(DirectX::XMVectorGetIntY(u._vec));
//    float u_z = static_cast<float>(DirectX::XMVectorGetIntZ(u._vec));
//
//
//    return vec3(u_y * v_z - u_z * v_y,
//        u_z * v_x - u_x * v_z,
//        u_x * v_y - u_y * v_x);
//}
//
//inline vec3 unit_vector(vec3 v) {
//    return v / v.length();
//}
//
////inline vec3 random_in_unit_disk() {
////    while (true) {
////        auto p = vec3(random_float(-1, 1), random_float(-1, 1), 0);
////        if (p.length_squared() >= 1) continue;
////        return p;
////    }
////}
//
//inline vec3 random_in_unit_sphere() {
//    while (true) {
//        auto p = vec3::random(-1, 1);
//        if (p.length_squared() >= 1) continue;
//        return p;
//    }
//}
//
//inline vec3 random_unit_vector() {
//    return unit_vector(random_in_unit_sphere());
//}
//
//inline vec3 random_in_hemisphere(const vec3& normal) {
//    vec3 in_unit_sphere = random_in_unit_sphere();
//    if (dot(in_unit_sphere, normal) > 0.0) // In the same hemisphere as the normal
//        return in_unit_sphere;
//    else
//        return -in_unit_sphere;
//}
//
//inline vec3 reflect(const vec3& v, const vec3& n) {
//    return v - 2 * dot(v, n) * n;
//}
//
//inline vec3 refract(const vec3& uv, const vec3& n, float etai_over_etat) {
//    auto cos_theta = fmin(dot(-uv, n), 1.0f);
//    vec3 r_out_perp = etai_over_etat * (uv + cos_theta * n);
//    vec3 r_out_parallel = -sqrt(fabs(1.0f - r_out_perp.length_squared())) * n;
//    return r_out_perp + r_out_parallel;
//}
//


//
//class vec3 {
//public:
//    vec3() : e{ 0,0,0 } {}
//    vec3(float e0, float e1, float e2) : e{ e0, e1, e2 } {}
//
//    float x() const { return e[0]; }
//    float y() const { return e[1]; }
//    float z() const { return e[2]; }
//
//    vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
//    float operator[](int i) const { return e[i]; }
//    float& operator[](int i) { return e[i]; }
//
//    vec3& operator+=(const vec3& v) {
//        e[0] += v.e[0];
//        e[1] += v.e[1];
//        e[2] += v.e[2];
//        return *this;
//    }
//
//    vec3& operator*=(const float t) {
//        e[0] *= t;
//        e[1] *= t;
//        e[2] *= t;
//        return *this;
//    }
//
//    vec3& operator/=(const float t) {
//        return *this *= 1 / t;
//    }
//
//    float length() const {
//        return sqrt(length_squared());
//    }
//
//    float length_squared() const {
//        return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
//    }
//
//    bool near_zero() const {
//        // Return true if the vector is close to zero in all dimensions.
//        const auto s = 1e-8;
//        return (fabs(e[0]) < s) && (fabs(e[1]) < s) && (fabs(e[2]) < s);
//    }
//
//    inline static vec3 random() {
//        return vec3(random_float(), random_float(), random_float());
//    }
//
//    inline static vec3 random(float min, float max) {
//        return vec3(random_float(min, max), random_float(min, max), random_float(min, max));
//    }
//
//public:
//    float e[3];
//};
//
//
//// Type aliases for vec3
//using point3 = vec3;   // 3D point
//using color = vec3;    // RGB color
//
//
//// vec3 Utility Functions
//
//inline std::ostream& operator<<(std::ostream& out, const vec3& v) {
//    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
//}
//
//inline vec3 operator+(const vec3& u, const vec3& v) {
//    return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
//}
//
//inline vec3 operator-(const vec3& u, const vec3& v) {
//    return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
//}
//
//inline vec3 operator*(const vec3& u, const vec3& v) {
//    return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
//}
//
//inline vec3 operator*(float t, const vec3& v) {
//    return vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
//}
//
//inline vec3 operator*(const vec3& v, float t) {
//    return t * v;
//}
//
//inline vec3 operator/(vec3 v, float t) {
//    return (1 / t) * v;
//}
//
//inline float dot(const vec3& u, const vec3& v) {
//    return u.e[0] * v.e[0]
//        + u.e[1] * v.e[1]
//        + u.e[2] * v.e[2];
//}
//
//inline vec3 cross(const vec3& u, const vec3& v) {
//    return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
//        u.e[2] * v.e[0] - u.e[0] * v.e[2],
//        u.e[0] * v.e[1] - u.e[1] * v.e[0]);
//}
//
//inline vec3 unit_vector(vec3 v) {
//    return v / v.length();
//}
//
//inline vec3 random_in_unit_disk() {
//    while (true) {
//        auto p = vec3(random_float(-1, 1), random_float(-1, 1), 0);
//        if (p.length_squared() >= 1) continue;
//        return p;
//    }
//}
//
//inline vec3 random_in_unit_sphere() {
//    while (true) {
//        auto p = vec3::random(-1, 1);
//        if (p.length_squared() >= 1) continue;
//        return p;
//    }
//}
//
//inline vec3 random_unit_vector() {
//    return unit_vector(random_in_unit_sphere());
//}
//
//inline vec3 random_in_hemisphere(const vec3& normal) {
//    vec3 in_unit_sphere = random_in_unit_sphere();
//    if (dot(in_unit_sphere, normal) > 0.0) // In the same hemisphere as the normal
//        return in_unit_sphere;
//    else
//        return -in_unit_sphere;
//}
//
//inline vec3 reflect(const vec3& v, const vec3& n) {
//    return v - 2 * dot(v, n) * n;
//}
//
//inline vec3 refract(const vec3& uv, const vec3& n, float etai_over_etat) {
//    auto cos_theta = fmin(dot(-uv, n), 1.0);
//    vec3 r_out_perp = etai_over_etat * (uv + cos_theta * n);
//    vec3 r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.length_squared())) * n;
//    return r_out_perp + r_out_parallel;
//}