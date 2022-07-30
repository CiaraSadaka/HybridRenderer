
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
        rcCamera(point3 lookfrom, point3 lookat, vec3 vup, float vfov, float aspect_ratio, float aperture, float focus_dist) {
         //   auto aspect_ratio = 16.0f / 9.0f;
            auto theta = degrees_to_radians(vfov);
            auto h = tan(theta / 2.0f);
            auto viewport_height = 2.0f * h;
            auto viewport_width = aspect_ratio * viewport_height;
           // auto focal_length = 1.0f;

             w = unit_vector(lookfrom - lookat);
             u = unit_vector(cross(vup, w));
             v = cross(w, u);

           _origin = lookfrom;
           horizontal = focus_dist * viewport_width * u;
           vertical = focus_dist * viewport_height * v;
           lower_left_corner = _origin - horizontal / 2.0f - vertical / 2.0f - focus_dist * w;

           lens_radius = aperture / 2.0f;
        }

       ray get_ray(float s, float t) const {
           vec3 rd = lens_radius * random_in_unit_disk();
           vec3 offset = u * rd.x() + v * rd.y();

           return ray(
               _origin + offset,
               lower_left_corner + s * horizontal + t * vertical - _origin - offset
           );
       }

    private:
        point3 _origin;
        point3 lower_left_corner;
        vec3 horizontal;
        vec3 vertical;
        vec3 u, v, w;
        float lens_radius;
    };

}