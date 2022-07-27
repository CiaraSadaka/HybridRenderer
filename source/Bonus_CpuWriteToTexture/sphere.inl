#include "pch.h"
#include "sphere.h"
//

namespace Rendering::Hybrid
{
// 
//    inline sphere::sphere(XMFLOAT4 cen, float r):
//        center(cen), radius(r)
//    {
//    }
//
//    inline bool sphere::hit(const Ray& r, 
//  t_min, double t_max, hit_record& rec) const 
//    {
//       /* r;
//        t_max;
//        rec;
//        t_min;*/
//           XMFLOAT4 o = r.origin();
//           XMVECTOR orig = XMLoadFloat4(&o);
//
//           XMVECTOR _center = XMLoadFloat4(&center);
//
//           XMVECTOR oc = orig - _center;
//           float a = length_squared(r.direction());
//
//           XMFLOAT4 _oc;
//           XMStoreFloat4(&_oc, oc);
//           float half_b = dot(_oc, r.direction());
//           float c = (float)(length_squared(_oc) - radius * radius);
//
//           float discriminant = half_b * half_b - a * c;
//           if (discriminant < 0) return false;
//           float sqrtd = sqrt(discriminant);
//
//           // Find the nearest root that lies in the acceptable range.
//           float root = (-half_b - sqrtd) / a;
//           if (root < t_min || t_max < root) {
//               root = (-half_b + sqrtd) / a;
//               if (root < t_min || t_max < root)
//                   return false;
//           }
//
//           rec.t = root;
//
//           XMFLOAT4 rAt;
//           XMStoreFloat4(&rAt,r.at(static_cast<float>(rec.t)));
//
//           rec.p = rAt;
//
//
//           XMVECTOR _p = XMLoadFloat4(&rec.p);
//
//         //  XMFLOAT4 outward_normal;
//
//        //   XMStoreFloat4(&outward_normal, (_p - _center) / radius);
//          // rec.normal =XMLoadFloat4(&outward_normal);
//           rec.set_face_normal(r, (_p - _center) / radius);
//           rec.mat_ptr = mat_ptr;
//           return true;
//    }
//
//
}