#pragma once

#include "vec3.h"

//using namespace Library;
using namespace DirectX;

namespace Rendering::Hybrid 
{
	class ray 
	{
	public:

		ray() = default;

		//~ray() = default;

		ray(const point3& origin, const vec3& direction)
			: _origin(origin), _dir(direction), tm(0)
		{}

		ray(const point3& origin, const vec3& direction, float time)
			: _origin(origin), _dir(direction), tm(time)
		{}

		point3 origin() const { return _origin; }
		vec3 direction() const { return _dir; }
		float time() const { return tm; }

		point3 at(float t) const {
			return _origin + t * _dir;
		}

	public:
		point3 _origin;
		vec3 _dir;
		float tm;
		
	};
}