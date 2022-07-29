
#pragma once

//#include "ray.h"
#include "RayHelper.h"
#include <DirectXMath.h>
//#include "VectorHelper.h"
//#include "material.h"
#include <memory>

//using namespace Raytracer;
//using namespace Library;
using namespace DirectX;

namespace Rendering::Hybrid //TODO: change namspce to libary
{
    class rcCamera {
        public:
        rcCamera() {
            auto aspect_ratio = 16.0f / 9.0f;
            auto viewport_height = 2.0f;
            auto viewport_width = aspect_ratio * viewport_height;
            auto focal_length = 1.0f;

            _origin = point3(0, 0, 0);
            horizontal = vec3(viewport_width, 0.0f, 0.0f);
            vertical = vec3(0.0f, viewport_height, 0.0f);
            lower_left_corner = _origin - horizontal / 2 - vertical / 2 - vec3(0, 0, focal_length);
        }

       ray get_ray(float u, float v) const {
            return ray(_origin, lower_left_corner + u * horizontal + (1-v) * vertical - _origin);//possible error, v-1 or not?
        }

    private:
        point3 _origin;
        point3 lower_left_corner;
        vec3 horizontal;
        vec3 vertical;
    };

}