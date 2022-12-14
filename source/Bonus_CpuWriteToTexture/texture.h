#pragma once
#include "RayHelper.h"
#include "perlin.h"

namespace Rendering::Hybrid {
    class texture {
    public:
        virtual color value(float u, float v, const point3& p) const = 0;
    };


    class solid_color : public texture {
    public:
        solid_color() {}
        solid_color(color c) : color_value(c) {}

        solid_color(float red, float green, float blue)
            : solid_color(color(red, green, blue)) {}

        virtual color value(float u, float v, const vec3& p) const override {
            u;
            v;
            p;
            return color_value;
        }

    private:
        color color_value;
    };

    class noise_texture : public texture {
    public:
        noise_texture() {}
        noise_texture(float sc) : scale(sc) {}

        virtual color value(float u, float v, const point3& p) const override {
            u;
            v;
            p;
            return color(1.0f, 1.0f, 1.0f) * 0.5f * (1.0f + sin(scale * p.z() + 10.0f * noise.turb(p)));
        }
    public:
        perlin noise;
        float scale;
    };

    class checker_texture : public texture {
    public:
        checker_texture() {}

        checker_texture(shared_ptr<texture> _even, shared_ptr<texture> _odd)
            : even(_even), odd(_odd) {}

        checker_texture(color c1, color c2)
            : even(make_shared<solid_color>(c1)), odd(make_shared<solid_color>(c2)) {}

        virtual color value(float u, float v, const point3& p) const override {
            auto sines = sin(10 * p.x()) * sin(10 * p.y()) * sin(10 * p.z());
            if (sines < 0)
                return odd->value(u, v, p);
            else
                return even->value(u, v, p);
        }

    public:
        shared_ptr<texture> odd;
        shared_ptr<texture> even;
    };
}