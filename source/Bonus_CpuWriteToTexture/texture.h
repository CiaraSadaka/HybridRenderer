#pragma once
#include "RayHelper.h"
#include "perlin.h"

#pragma warning (push, 0)
#include "stb_image_read.h"
#pragma warning (pop)
//#include <iostream>
//#include "stb_image_read.h"
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

    class RTimage_texture : public texture {
    public:
        const static int bytes_per_pixel = 3;

        RTimage_texture()
            : data(nullptr), width(0), height(0), bytes_per_scanline(0) {}

        RTimage_texture(const char* filename) {
            auto components_per_pixel = bytes_per_pixel;

            data = stbi_load(
                filename, &width, &height, &components_per_pixel, components_per_pixel);

            if (!data) {
                std::cerr << "ERROR: Could not load texture image file '" << filename << "'.\n";
                width = height = 0;
            }

            bytes_per_scanline = bytes_per_pixel * width;
        }

        ~RTimage_texture() {
            delete data;
        }

        virtual color value(float u, float v, const vec3& p) const override {
            p;
            // If we have no texture data, then return solid cyan as a debugging aid.
            if (data == nullptr)
                return color(0.0f, 1.0f, 1.0f);

            // Clamp input texture coordinates to [0,1] x [1,0]
            u = clamp(u, 0.0f, 1.0f);
            v = 1.0f - clamp(v, 0.0f, 1.0f);  // Flip V to image coordinates

            auto i = static_cast<int>(u * width);
            auto j = static_cast<int>(v * height);

            // Clamp integer mapping, since actual coordinates should be less than 1.0
            if (i >= width)  i = width - 1;
            if (j >= height) j = height - 1;

            const auto color_scale = 1.0f / 255.0f;
            auto pixel = data + j * bytes_per_scanline + i * bytes_per_pixel;

            return color(color_scale * pixel[0], color_scale * pixel[1], color_scale * pixel[2]);
        }

    private:
        unsigned char* data;
        int width, height;
        int bytes_per_scanline;
    };
}