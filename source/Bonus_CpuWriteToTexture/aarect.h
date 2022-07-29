#pragma once
#include "RayHelper.h"
#include "hittable.h"

using namespace DirectX;

namespace Rendering::Hybrid
{
    class xy_rect : public hittable
    {
    public:
        xy_rect() {}

        xy_rect(float _x0, float _x1, float _y0, float _y1, float _k,
            shared_ptr<material> mat)
            : x0(_x0), x1(_x1), y0(_y0), y1(_y1), k(_k), mp(mat) {};

        virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const override;

        virtual bool bounding_box(float time0, float time1, aabb& output_box) const override {
            // The bounding box must have non-zero width in each dimension, so pad the Z
            // dimension a small amount.
            output_box = aabb(point3(x0, y0, k - 0.0001), point3(x1, y1, k + 0.0001));
            return true;
        }

    public:
        shared_ptr<material> mp;
        float x0, x1, y0, y1, k;
    };
}