#pragma once
#include "RayHelper.h"
#include "hittable.h"

namespace Rendering::Hybrid
{
    class xy_rect : public hittable {
    public:
        xy_rect() {}

        xy_rect(float _x0, float _x1, float _y0, float _y1, float _k,
            shared_ptr<material> mat)
            : x0(_x0), x1(_x1), y0(_y0), y1(_y1), k(_k), mp(mat) {};

        virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const override;

        virtual bool bounding_box(float time0, float time1, aabb& output_box) const override {
            time0;
            time1;
            // The bounding box must have non-zero width in each dimension, so pad the Z
            // dimension a small amount.
            output_box = aabb(point3(x0, y0, k - 0.0001f), point3(x1, y1, k + 0.0001f));
            return true;
        }

    public:
        shared_ptr<material> mp;
        float x0, x1, y0, y1, k;
    };

    inline bool xy_rect::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
        auto t = (k - r.origin().z()) / r.direction().z();
        if (t < t_min || t > t_max)
            return false;
        auto x = r.origin().x() + t * r.direction().x();
        auto y = r.origin().y() + t * r.direction().y();
        if (x < x0 || x > x1 || y < y0 || y > y1)
            return false;
        rec.u = (x - x0) / (x1 - x0);
        rec.v = (y - y0) / (y1 - y0);
        rec.t = t;
        auto outward_normal = vec3(0, 0, 1);
        rec.set_face_normal(r, outward_normal);
        rec.mat_ptr = mp;
        rec.p = r.at(t);
        return true;
    }

    class xz_rect : public hittable {
    public:
        xz_rect() {}

        xz_rect(float _x0, float _x1, float _z0, float _z1, float _k,
            shared_ptr<material> mat)
            : x0(_x0), x1(_x1), z0(_z0), z1(_z1), k(_k), mp(mat) {};

        virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const override;

        virtual bool bounding_box(float time0, float time1, aabb& output_box) const override {
            time0;
            time1;
            // The bounding box must have non-zero width in each dimension, so pad the Y
            // dimension a small amount.
            output_box = aabb(point3(x0, k - 0.0001f, z0), point3(x1, k + 0.0001f, z1));
            return true;
        }

    public:
        shared_ptr<material> mp;
        float x0, x1, z0, z1, k;
    };

    class yz_rect : public hittable {
    public:
        yz_rect() {}

        yz_rect(float _y0, float _y1, float _z0, float _z1, float _k,
            shared_ptr<material> mat)
            : y0(_y0), y1(_y1), z0(_z0), z1(_z1), k(_k), mp(mat) {};

        virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const override;

        virtual bool bounding_box(float time0, float time1, aabb& output_box) const override {
            time0;
            time1;
            // The bounding box must have non-zero width in each dimension, so pad the X
            // dimension a small amount.
            output_box = aabb(point3(k - 0.0001f, y0, z0), point3(k + 0.0001f, y1, z1));
            return true;
        }

    public:
        shared_ptr<material> mp;
        float y0, y1, z0, z1, k;
    };

    inline bool xz_rect::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
        auto t = (k - r.origin().y()) / r.direction().y();
        if (t < t_min || t > t_max)
            return false;
        auto x = r.origin().x() + t * r.direction().x();
        auto z = r.origin().z() + t * r.direction().z();
        if (x < x0 || x > x1 || z < z0 || z > z1)
            return false;
        rec.u = (x - x0) / (x1 - x0);
        rec.v = (z - z0) / (z1 - z0);
        rec.t = t;
        auto outward_normal = vec3(0.0f, 1.0f, 0.0f);
        rec.set_face_normal(r, outward_normal);
        rec.mat_ptr = mp;
        rec.p = r.at(t);
        return true;
    }

    inline bool yz_rect::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
        auto t = (k - r.origin().x()) / r.direction().x();
        if (t < t_min || t > t_max)
            return false;
        auto y = r.origin().y() + t * r.direction().y();
        auto z = r.origin().z() + t * r.direction().z();
        if (y < y0 || y > y1 || z < z0 || z > z1)
            return false;
        rec.u = (y - y0) / (y1 - y0);
        rec.v = (z - z0) / (z1 - z0);
        rec.t = t;
        auto outward_normal = vec3(1.0f, 0.0f, 0.0f);
        rec.set_face_normal(r, outward_normal);
        rec.mat_ptr = mp;
        rec.p = r.at(t);
        return true;
    }
}