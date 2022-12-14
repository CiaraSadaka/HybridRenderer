#pragma once

//#include "CpuWriteToTextureDemo.h"
//#include "vec3.h"
#include "RayHelper.h"
#include "hittable.h"
#include "texture.h"
//#include "Ray.h"
//#include <DirectXMath.h>
//#include "hittable.h"

//using namespace Library;
//using namespace Library;
using namespace DirectX;

namespace Rendering::Hybrid
{
    struct hit_record;


    class material {
    public:
        virtual color emitted(float u, float v, const point3& p) const {
            p;
            v;
            u;
            return color(0.0f, 0.0f, 0.0f);
        }
        virtual bool scatter(
            const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
        ) const = 0;
    };

    class diffuse_light : public material {
    public:
        diffuse_light(shared_ptr<texture> a) : emit(a) {}
        diffuse_light(color c) : emit(make_shared<solid_color>(c)) {}

        virtual bool scatter(
            const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
        ) const override {
            r_in;
            rec;
            attenuation;
            scattered;
            return false;
        }

        virtual color emitted(float u, float v, const point3& p) const override {
            return emit->value(u, v, p);
        }

    public:
        shared_ptr<texture> emit;
    };

    class lambertian : public material {
    public:
        lambertian(const color& a) : albedo(make_shared<solid_color>(a)) {}
        lambertian(shared_ptr<texture> a) : albedo(a) {}

        virtual bool scatter(
            const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
        ) const override {
            auto scatter_direction = rec.normal + random_unit_vector();

            // Catch degenerate scatter direction
            if (scatter_direction.near_zero())
                scatter_direction = rec.normal;

            scattered = ray(rec.p, scatter_direction, r_in.time());
            attenuation = albedo->value(rec.u, rec.v, rec.p);
            return true;
        }

    public:
        shared_ptr<texture> albedo;
    };


    class metal : public material {
    public:
        metal(const color& a, float f) : albedo(a), fuzz(f < 1 ? f : 1) {}

        virtual bool scatter(
            const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
        ) const override {
            vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
            scattered = ray(rec.p, reflected + fuzz * random_in_unit_sphere());
            attenuation = albedo;
            return (dot(scattered.direction(), rec.normal) > 0);
        }

    public:
        color albedo;
        float fuzz;
    };

    class isotropic : public material {
    public:
        isotropic(color c) : albedo(make_shared<solid_color>(c)) {}
        isotropic(shared_ptr<texture> a) : albedo(a) {}

        virtual bool scatter(
            const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
        ) const override {
            scattered = ray(rec.p, random_in_unit_sphere(), r_in.time());
            attenuation = albedo->value(rec.u, rec.v, rec.p);
            return true;
        }

    public:
        shared_ptr<texture> albedo;
    };

    class dielectric : public material {
    public:
        dielectric(float index_of_refraction) : ir(index_of_refraction) {}

        virtual bool scatter(
            const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
        ) const override {
            attenuation = color(1.0, 1.0, 1.0);
            float refraction_ratio = rec.front_face ? (1.0f / ir) : ir;

            vec3 unit_direction = unit_vector(r_in.direction());
            float cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0f);
            float sin_theta = sqrt(1.0f - cos_theta * cos_theta);

            bool cannot_refract = refraction_ratio * sin_theta > 1.0;
            vec3 direction;

            if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_float())
                direction = reflect(unit_direction, rec.normal);
            else
                direction = refract(unit_direction, rec.normal, refraction_ratio);

            scattered = ray(rec.p, direction);
            return true;
        }

    public:
        float ir; // Index of Refraction

    private:
        static float reflectance(float cosine, float ref_idx) {
            // Use Schlick's approximation for reflectance.
            auto r0 = (1 - ref_idx) / (1 + ref_idx);
            r0 = r0 * r0;
            return r0 + (1.0f - r0) * pow((1.0f - cosine), 5.0f);
        }
    };


}
