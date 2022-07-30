#include "pch.h"
#include "CpuWriteToTextureDemo.h"

using namespace std;
using namespace std::string_literals;
using namespace std::chrono;
using namespace gsl;
using namespace winrt;
using namespace Library;
using namespace DirectX;
using namespace Rendering::Hybrid;


namespace Rendering
{
	//using point3 = XMFLOAT3;
	//using vec3 = XMFLOAT3;
	//using color = XMFLOAT3;

	CpuWriteToTextureDemo::CpuWriteToTextureDemo(Game& game, const shared_ptr<Camera>& camera) :
		DrawableGameComponent(game, camera),
		_fullScreenQuad(game)
	{
	}

	CpuWriteToTextureDemo::~CpuWriteToTextureDemo()
	{
	}

	void CpuWriteToTextureDemo::Initialize()
	{

		const auto renderTargetSize = mGame->RenderTargetSize();
		D3D11_TEXTURE2D_DESC textureDesc;
		ZeroMemory(&textureDesc, sizeof(textureDesc));
		//set the wincdow size 
		textureDesc.Width = renderTargetSize.cx;
		textureDesc.Height = renderTargetSize.cy;

		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		//this stuff tells the gpu that the textures will change
		textureDesc.Usage = D3D11_USAGE_DYNAMIC;
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		_colorMap = Texture2D::CreateTexture2D(mGame->Direct3DDevice(), textureDesc);

		_fullScreenQuad.Initialize();
		auto fullScreenQuadMaterial = _fullScreenQuad.Material();
		fullScreenQuadMaterial->SetTexture(_colorMap->ShaderResourceView().get());

		auto pixelShader = mGame->Content().Load<PixelShader>(L"Shaders\\TexturedModelPS.cso");
		fullScreenQuadMaterial->SetShader(pixelShader);

		CreateImageBuffer();
	}

	void CpuWriteToTextureDemo::Draw(const GameTime& gameTime)
	{
		if (high_resolution_clock::now() > _lastTextureUpdate + _textureUpdateDelay)
		{
			com_ptr<ID3D11Resource> colorMapResource;
			_colorMap->ShaderResourceView()->GetResource(colorMapResource.put());
			D3D11_MAPPED_SUBRESOURCE mappedResource;
			ThrowIfFailed(mGame->Direct3DDeviceContext()->Map(colorMapResource.get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));
			
			computeRTImage(ImageBuffer.get());

		/*	const auto width = static_cast<unsigned int>(mGame->RenderTargetSize().cx);
			memcpy(mappedResource.pData , ImageBuffer.get()+ (width * (jCounter+1)), width * sizeof(uint8_t) * 4);*/

			/*const auto width = static_cast<unsigned int>(mGame->RenderTargetSize().cx);
			uint8_t* src = ImageBuffer.get(), * dest = reinterpret_cast<uint8_t*>(mappedResource.pData);
			memcpy(dest, src, width * 4 * sizeof(uint8_t));
			dest += mappedResource.RowPitch;
			src += width * 4;*/

			if (const auto width = static_cast<unsigned int>(mGame->RenderTargetSize().cx); mappedResource.RowPitch == width * 4) {
				memcpy(mappedResource.pData, ImageBuffer.get(), BufferSize() * sizeof(uint8_t));
			}
			else {
				uint8_t* src = ImageBuffer.get(), * dest = reinterpret_cast<uint8_t*>(mappedResource.pData);
				for (int iy = 0; iy < mGame->RenderTargetSize().cy; ++iy) {
					memcpy(dest, src, width * 4 * sizeof(uint8_t));
					dest += mappedResource.RowPitch;
					src += width * 4;
				}
			}


			mGame->Direct3DDeviceContext()->Unmap(colorMapResource.get(), 0);
			
			_lastTextureUpdate = high_resolution_clock::now(); //update delay of one sec but need to change to everytime i write a full row (or ever second)
		}

		_fullScreenQuad.Draw(gameTime);
		_fullScreenQuad.Material()->UnbindShaderResources<1>(ShaderStages::PS);
	}

	void CpuWriteToTextureDemo::WritePixelToBuffer(uint8_t* buffer, unsigned int ix, unsigned int iy, unsigned int samples_per_pixel,color pixel_color) 
	{

		iy = mGame->RenderTargetSize().cy - 1 - iy;
		auto r = pixel_color.x();
		auto g = pixel_color.y();
		auto b = pixel_color.z();



		////TODO: READD THIS
		//if (r != r) r = 0.0;
		//if (g != g) g = 0.0;
		//if (b != b) b = 0.0;

		float scale = 1.0f / samples_per_pixel;

		r = clamp(sqrt(r * scale), 0.0f, 0.999f);
		g = clamp(sqrt(g * scale), 0.0f, 0.999f);
		b = clamp(sqrt(b * scale), 0.0f, 0.999f);

		
		const unsigned int idx = 4 * (ix + iy * mGame->RenderTargetSize().cx);

		buffer[idx] = static_cast<uint8_t>(255 * r);
		buffer[idx + 1] = static_cast<uint8_t>(255 * g);
		buffer[idx + 2] = static_cast<uint8_t>(255 * b);
		buffer[idx + 3] = 255;

	}

	size_t CpuWriteToTextureDemo::BufferSize() const
	{
		return static_cast<size_t>(mGame->RenderTargetSize().cx) * static_cast<size_t>(mGame->RenderTargetSize().cy) * 4l;
	}

	void CpuWriteToTextureDemo::CreateImageBuffer() 
	{
		auto newBuffer = std::unique_ptr<uint8_t[]>(new uint8_t[BufferSize()]());
		ImageBuffer.swap(newBuffer);
	}

	void CpuWriteToTextureDemo::computeRTImage(uint8_t* buffer)
	{
		// render blue white gradient

		// Image
		//const float aspect_ratio = (float)(16.0 / 12.0);
		const int image_width = mGame->RenderTargetSize().cx;
		//const int image_height = mGame->RenderTargetSize().cy;

		const float aspect_ratio = (float)(3.0 / 2.0);

		//
		
		// World
	


		//hittable_list world;

		//auto material_ground = make_shared<lambertian>(color(0.8f, 0.8f, 0.0f));
		//auto material_center = make_shared<lambertian>(color(0.1f, 0.2f, 0.5f));
		//auto material_left = make_shared<dielectric>(1.5f);
		//auto material_right = make_shared<metal>(color(0.8f, 0.6f, 0.2f), 0.0f);

		//world.add(make_shared<sphere>(point3(0.0f, -100.5f, -1.0f), 100.0f, material_ground));
		//world.add(make_shared<sphere>(point3(0.0f, 0.0f, -1.0f), 0.5f, material_center));
		//world.add(make_shared<sphere>(point3(-1.0f, 0.0f, -1.0f), 0.5f, material_left));
		//world.add(make_shared<sphere>(point3(-1.0f, 0.0f, -1.0f), -0.45f, material_left));
		//world.add(make_shared<sphere>(point3(1.0f, 0.0f, -1.0f), 0.5f, material_right));
		
		
		auto world = random_scene();

		point3 lookfrom(13.0f, 2.0f, 3.0f);
		point3 lookat(0.0f, 0.0f, -1.0f);
		vec3 vup(0.0f, 1.0f, 0.0f);
		auto dist_to_focus = 10.0f;
		auto aperture = 0.1f;

		rcCamera cam(lookfrom, lookat, vup, 20.0f, aspect_ratio, aperture, dist_to_focus);
		//rcCamera cam(point3(-2.0f, 2.0f, 1.0f), point3(0.0f, 0.0f, -1.0f), vec3(0.0f, 1.0f, 0.0f), 90.0f, aspect_ratio);
	//	rcCamera cam(point3(-2.0f, 2.0f, 1.0f), point3(0.0f, 0.0f, -1.0f), vec3(0.0f, 1.0f, 0.0f), 20.0f, aspect_ratio);
		//auto R = cos(pi / 4.0f);

		//hittable_list world;

		//auto material_left = make_shared<lambertian>(color(0.0f, 0.0f, 1.0f));
		//auto material_right = make_shared<lambertian>(color(1.0f, 0.0f, 0.0f));

		//world.add(make_shared<sphere>(point3(-R, 0.0f, -1.0f), R, material_left));
		//world.add(make_shared<sphere>(point3(R, 0.0f, -1.0f), R, material_right));

		//// Camera

		//rcCamera cam(90.0f, aspect_ratio);



		//world.add(make_shared<sphere>(XMFLOAT4(0, 0, -1, 0), static_cast <float>(0.5)));
		//world.add(make_shared<sphere>(XMFLOAT4(0, -100.5, -1, 0), static_cast <float>(100)));


		/*auto material_ground = make_shared<lambertian>(color(0.8f, 0.8f, 0.0f));
		auto material_center = make_shared<dielectric>(1.5f);
		auto material_left = make_shared<dielectric>(1.5f);
		auto material_right = make_shared<metal>(color(0.8f, 0.6f, 0.2f), 1.0f);

		world.add(make_shared<sphere>(point3(0.0f, -100.5f, -1.0f), static_cast <float>(100.0f), material_ground));
		world.add(make_shared<sphere>(point3(0.0f, 0.0f, -1.0f), static_cast <float>(0.5f), material_center));
		world.add(make_shared<sphere>(point3(-1.0f, 0.0f, -1.0f), static_cast <float>(0.5f), material_left));
		world.add(make_shared<sphere>(point3(1.0f, 0.0f, -1.0f), static_cast <float>(0.5f), material_right));*/



		/*auto material1 = make_shared<dielectric>(1.5f);
		world.add(make_shared<sphere>(point3(0.0f, 1.0f, 0.0f), 1.0f, material1));

		auto material2 = make_shared<lambertian>(color(0.4f, 0.2f, 0.1f));
		world.add(make_shared<sphere>(point3(-4.0f, 1.0f, 0.0f), 1.0f, material2));

		auto material3 = make_shared<metal>(color(0.7f, 0.6f, 0.5f), 0.0f);
		world.add(make_shared<sphere>(point3(4.0f, 1.0f, 0.0f), 1.0f, material3));*/





		// Camera
		//cam center at (0,0,0) and y go in to the right

		//float viewport_height = 2.0;
		//float viewport_width = aspect_ratio * viewport_height;
		//float focal_length = 1.0;
		//XMFLOAT4 _origin(0 , 0 ,0, 0);
		//auto origin = XMLoadFloat4(&_origin);
		//XMFLOAT4 _horizontal(viewport_width, 0, 0, 0);
		//auto horizontal = XMLoadFloat4(&_horizontal);
		//XMFLOAT4 _vertical(0, viewport_height, 0, 0);
		//auto vertical = XMLoadFloat4(&_vertical);
		//XMFLOAT4 _FL(0, 0, focal_length, 0);
		//auto FL = XMLoadFloat4(&_FL);
		//auto lower_left_corner = origin - horizontal / 2 - vertical / 2 - FL;

		//point3 lookfrom(13, 2, 3);
		//point3 lookat(0, 0, 0);
		//vec3 vup(0, 1, 0);
		////auto dist_to_focus = 10.0;
		////auto aperture = 0.1;
		//
		//rcCamera cam;

		if (jCounter == 0)
		{
			jCounter = mGame->RenderTargetSize().cy - 1;
		}
		int j = jCounter;


			for (int i = 0; i < mGame->RenderTargetSize().cx; ++i)
			{
			
				color pixel_color(0, 0, 0);
				
				for (int s = 0; s < samples_per_pix; ++s) 
				{
					auto u = (i + random_float()) / (mGame->RenderTargetSize().cx - 1);
					auto v = (j + random_float()) / (mGame->RenderTargetSize().cy - 1);
					ray r;
					 r = cam.get_ray(u, v);
					 
					 vec3 c = ray_color(r, world, max_depth);
					
					pixel_color += c;
				}
				WritePixelToBuffer(buffer, i, j, samples_per_pix, pixel_color);


			}
		jCounter--;
	
	}

	
	color CpuWriteToTextureDemo::ray_color(const Hybrid::ray& r, const Hybrid::hittable& world, int depth)
	{
		
		hit_record rec;
		// If we've exceeded the ray bounce limit, no more light is gathered.
		if (depth <= 0)
			return color(0, 0, 0);

		if (world.hit(r, 0.001f, infinity, rec)) {
			ray scattered;
			color attenuation;
			if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
				return attenuation * ray_color(scattered, world, depth - 1);
			return color(0, 0, 0);
		}

		vec3 unit_direction = unit_vector(r.direction());
		float t = 0.5f * (unit_direction.y() + 1.0f);
		color toReturn = (1.0f - t) * color(1.0f, 1.0f, 1.0f) + t * color(0.5f, 0.7f, 1.0f);
		return toReturn;
			
	}

	hittable_list CpuWriteToTextureDemo::random_scene() {
		hittable_list world;

		auto ground_material = make_shared<lambertian>(color(0.5f, 0.5f, 0.5f));
		world.add(make_shared<sphere>(point3(0.0f, -1000.0f, 0.0f), 1000.0f, ground_material));

		for (int a = -11; a < 11; a++) {
			for (int b = -11; b < 11; b++) {
				auto choose_mat = random_float();
				point3 center(a + 0.9f * random_float(), 0.2f, b + 0.9f * random_float());

				if ((center - point3(4.0f, 0.2f, 0.0f)).length() > 0.9f) {
					shared_ptr<material> sphere_material;

					if (choose_mat < 0.8) {
						// diffuse
						auto albedo = color::random() * color::random();
						sphere_material = make_shared<lambertian>(albedo);
						world.add(make_shared<sphere>(center, 0.2f, sphere_material));
					}
					else if (choose_mat < 0.95) {
						// metal
						auto albedo = color::random(0.5, 1.0);
						auto fuzz = random_float(0, 0.5);
						sphere_material = make_shared<metal>(albedo, fuzz);
						world.add(make_shared<sphere>(center, 0.2f, sphere_material));
					}
					else {
						// glass
						sphere_material = make_shared<dielectric>(1.5f);
						world.add(make_shared<sphere>(center, 0.2f, sphere_material));
					}
				}
			}
		}

		auto material1 = make_shared<dielectric>(1.5f);
		world.add(make_shared<sphere>(point3(0.0f, 1.0f, 0.0f), 1.0f, material1));

		auto material2 = make_shared<lambertian>(color(0.4f, 0.2f, 0.1f));
		world.add(make_shared<sphere>(point3(-4.0f, 1.0f, 0.0f), 1.0f, material2));

		auto material3 = make_shared<metal>(color(0.7f, 0.6f, 0.5f), 0.0f);
		world.add(make_shared<sphere>(point3(4.0f, 1.0f, 0.0f), 1.0f, material3));

		return world;
	}



	//color CpuWriteToTextureDemo::ray_color(const Ray& r)
	//{
	//	r;
	//	float t = hit_sphere(point4(0, 0, -1, 1), 0.5, r);
	//	/*if (t > 0.0) {

	//		XMFLOAT4 t1(0, 0, -1, 1);
	//		auto _t1 = XMLoadFloat4(&t1);
	//		
	//		XMVECTOR toUnit =  r.at(t) - _t1;
	//		XMStoreFloat4(&t1, toUnit);
	//		XMFLOAT4 N = unit_vector(t1);

	//		XMFLOAT4 _color(N.x + 1, N.y + 1, N.z + 1, 1);
	//		_color.x *= 0.5;
	//		_color.y *= 0.5;
	//		_color.z *= 0.5;
	//		_color.w *= 0.5;

	//		return _color;
	//	}
	//	XMFLOAT4 unit_direction = unit_vector(r.direction());
	//	t = (float)0.5 * (float)(unit_direction.y + 1.0);
	//	
	//	t = (float) 1.0 - t; 
	//	XMFLOAT4 t2((float)1.0*((float)1.0-t), (float)1.0*((float)1.0 - t), (float)1.0*((float)1.0 - t), (float)1.0*((float)1.0 - t));

	//	XMFLOAT4 t3((float)0.5*t, (float)0.7*t, (float)1.0*t, (float)1.0*t);

	//	t2.x += t3.x;
	//	t2.y += t3.y;
	//	t2.z += t3.z;
	//	t2.w += t3.w;*/

	//	//return t2;

	//	return color(1, 1, 1, 1);
	//}

	//inline XMFLOAT4 CpuWriteToTextureDemo::unit_vector(XMFLOAT4 v)
	//{
	//	XMFLOAT4 unitV;
	//	unitV.x = v.x / length(v);
	//	unitV.y = v.y / length(v);
	//	unitV.z = v.z / length(v);
	//	return  unitV;
	//}

	//inline float CpuWriteToTextureDemo::length(XMFLOAT4 _values) const
	//{
	//	return sqrtf(length_squared(_values));
	//}

	//inline float CpuWriteToTextureDemo::length_squared(XMFLOAT4 _values) const
	//{
	//	return _values.x * _values.x + _values.y * _values.y + _values.z * _values.z + _values.w * _values.w;
	//}

	//inline float CpuWriteToTextureDemo::dot(const XMFLOAT4& u, const XMFLOAT4& v)
	//{
	//	return u.x * v.x
	//		+ u.y * v.y
	//		+ u.z * v.z;
	//	//	+ u[3] * v[3];
	//}

	

}


	


