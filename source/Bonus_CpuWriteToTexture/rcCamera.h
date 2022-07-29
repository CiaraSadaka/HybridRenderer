
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

            rcCamera() : rcCamera(point3(0, 0, -1), point3(0, 0, 0), vec3(0, 1, 0), 40, 1, 0, 10) {}

            rcCamera(
                    point3 lookfrom,
                    point3 lookat,
                    vec3   vup,
                    float vfov, // vertical field-of-view in degrees
                    float aspect_ratio,
                    float aperture,
                    float focus_dist
                ) {
                auto theta = degrees_to_radians(vfov);
                float h = tan(theta / 2.0f);
                float viewport_height = 2.0f * h;
                float viewport_width = aspect_ratio * viewport_height;

            /*    w = unit_vector(lookfrom - lookat);
                u = unit_vector(cross(vup, w));
                v = cross(w, u);

                origin = lookfrom;
                horizontal = focus_dist * viewport_width * u;
                vertical = focus_dist * viewport_height * v;
                lower_left_corner = origin - horizontal / 2.0f - vertical / 2.0f - focus_dist * w;

                lens_radius = aperture / 2.0f;*/
            

            auto focal_length = 1.0;

            origin = point3(0, 0, 0);
            horizontal = vec3(viewport_width, 0.0, 0.0);
            vertical = vec3(0.0, viewport_height, 0.0);
            lower_left_corner = origin - horizontal / 2 - vertical / 2 - vec3(0, 0, focal_length);
    }

             ray get_ray(double u, double v) const {
                 return ray(origin, lower_left_corner + u * horizontal + v * vertical - origin);
             }

          /*  ray get_ray(float s, float t) const {
                vec3 rd = lens_radius * random_in_unit_disk();
                vec3 offset = u * rd.x() + v * rd.y();

                return ray(
                    origin + offset,
                    lower_left_corner + s * horizontal + t * vertical - origin - offset
                );
            }*/
    

    private:
        //point3 origin;
        //point3 lower_left_corner;
        //vec3 horizontal;
        //vec3 vertical;
        //vec3 u, v, w;
        //float lens_radius;
        //float time0, time1;  // shutter open/close times
        point3 origin;
        point3 lower_left_corner;
        vec3 horizontal;
        vec3 vertical;
        vec3 u, v, w;
        float lens_radius;
    };

}