#pragma once

//#include "ray.h"
////#include "vec3.h"
#include <DirectXMath.h>
#include <random>
#include <cmath>
#include <cstdlib>
#include <limits>
#include <memory>

using std::shared_ptr;
using std::make_shared;
using std::sqrt;
using namespace DirectX;
//using namespace Rendering;
//using namespace Rendering::Hybrid;
////using namespace Library;


		// Constants

		const float infinity = std::numeric_limits<float>::infinity();
		const float pi = (float)3.1415926535897932385;

		// Utility Functions
        inline float clamp(float x, float min, float max) {
            if (x < min) return min;
            if (x > max) return max;
            return x;
        }

        inline float random_float() {

            return rand() / (RAND_MAX + 1.0f);
        }

        inline float random_float(float min, float max) {
            // Returns a random real in [min,max).
            return min + (max - min) * random_float();
        }

        inline int random_int(int min, int max) {
            // Returns a random integer in [min,max].
            return static_cast<int>(random_float(static_cast<float>(min), static_cast<float>(max) + 1.0f));
        }

		

#include "ray.h"
#include "vec3.h"