#pragma once

#include "pch.h"
#include <gsl\gsl>
#include <winrt\Windows.Foundation.h>
#include <d3d11.h>
#include <chrono>
#include "DrawableGameComponent.h"
#include "FullScreenQuad.h"
#include "Texture2D.h"
//#include "Ray.h"
#include <vector>
#include "VectorHelper.h"
#include "hittable.h"
#include "sphere.h"
//#include "RayHelper.h"

#include "CpuWriteToTextureDemo.h"
#include "Game.h"
#include "GameException.h"
#include "FullScreenQuadMaterial.h"
#include "PixelShader.h"
#include "DirectXTK/SimpleMath.h"
#include "ColorHelper.h"
//#include "hittable.h"
#include "hittable_list.h"
#include "hittable.h"
#include "sphere.h"
#include "RayHelper.h"
//#include "ray.h"
#include "material.h"
#include "rcCamera.h"
#include "aarect.h"
//#include "vec3.h"


//using std::shared_ptr;
//using std::make_shared;
//using std::sqrt;

using namespace std::chrono_literals;
using namespace Library;
using namespace Rendering::Hybrid;

namespace Rendering
{
	class CpuWriteToTextureDemo final : public Library::DrawableGameComponent
	{
	public:
		CpuWriteToTextureDemo(Library::Game& game, const std::shared_ptr<Library::Camera>& camera);
		CpuWriteToTextureDemo(const CpuWriteToTextureDemo&) = delete;
		CpuWriteToTextureDemo(CpuWriteToTextureDemo&&) = default;
		CpuWriteToTextureDemo& operator=(const CpuWriteToTextureDemo&) = default;		
		CpuWriteToTextureDemo& operator=(CpuWriteToTextureDemo&&) = default;
		~CpuWriteToTextureDemo();
		
		virtual void Initialize() override;
		virtual void Draw(const Library::GameTime& gameTime) override;

		void WritePixelToBuffer(uint8_t* buffer, unsigned int ix, unsigned int iy, unsigned int samples_per_pixel, color pixel_color);


		//makes a file called Sphere with a blue-grey gradient background and a Sphere.
		void computeRTImage(uint8_t* buffer);
	//	float hit_sphere(const vec3& center, float radius, const ray& r);

		size_t BufferSize() const;
		void CreateImageBuffer();
		color ray_color(const ray& r, const color& background, const hittable& world, int depth);
		hittable_list random_scene();	
		hittable_list two_spheres();
		hittable_list two_perlin_spheres();
		hittable_list simple_light();
		/*XMFLOAT4 unit_vector(XMFLOAT4 v);
		float dot(const XMFLOAT4& u, const XMFLOAT4& v);

		float length(XMFLOAT4 _values) const;

		float length_squared(XMFLOAT4 _values) const;*/

	private:


		int jCounter{0};
		int samples_per_pix{ 5};
		const int max_depth{ 10 };

		std::unique_ptr < uint8_t[] > ImageBuffer;
		inline static const std::chrono::seconds _textureUpdateDelay { 0};

		Library::FullScreenQuad _fullScreenQuad;
		std::shared_ptr<Library::Texture2D>	_colorMap;
		
		std::random_device _device;
		std::default_random_engine _generator{ _device() };
		std::uniform_int_distribution<uint32_t> _colorDistribution{ 0, 255 };
		std::chrono::high_resolution_clock::time_point _lastTextureUpdate;

	};



}