#pragma once

#include "hittable.h"
#include "material.h"
#include "CpuWriteToTextureDemo.h"
#include "VectorHelper.h"
#include "RayHelper.h"
#include <DirectXMath.h>
#include "ray.h"
#include "vec3.h"
//
using namespace DirectX;


namespace Rendering::Hybrid
{


    class sphere : public hittable {
    public:
        sphere() {}

        sphere(point3 cen, float r, shared_ptr<material> m)
            : center(cen), radius(r), mat_ptr(m) {};

        virtual bool hit(
            const ray& r, float t_min, float t_max, hit_record& rec) const override;

    public:
        point3 center;
        float radius;
        shared_ptr<material> mat_ptr;
    };


   inline bool sphere::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
        vec3 oc = r.origin() - center;
        auto a = r.direction().length_squared();
        auto half_b = dot(oc, r.direction());
        auto c = oc.length_squared() - radius * radius;

        auto discriminant = half_b * half_b - a * c;
        if (discriminant < 0) return false;
        auto sqrtd = sqrt(discriminant);

        // Find the nearest root that lies in the acceptable range.
        auto root = (-half_b - sqrtd) / a;
        if (root < t_min || t_max < root) {
            root = (-half_b + sqrtd) / a;
            if (root < t_min || t_max < root)
                return false;
        }

        rec.t = root;
        rec.p = r.at(rec.t);
        vec3 outward_normal = (rec.p - center) / radius;
        rec.set_face_normal(r, outward_normal);
        rec.mat_ptr = mat_ptr;

        return true;
    }
}

#include "sphere.inl"

