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

		const float aspect_ratio = (float)(16.0 / 9.0);


		
		// World
		hittable_list world;

		point3 lookfrom;
		point3 lookat;
		auto vfov = 40.0f;
		auto aperture = 0.0f;
		color background(0.0f, 0.0f, 0.0f);
		switch (0) {
		case 1:
			world = random_scene();
			background = color(0.70f, 0.80f, 1.00f);
			lookfrom = point3(13.0f, 2.0f, 3.0f);
			lookat = point3(0.0f, 0.0f, 0.0f);
			vfov = 20.0f;
			aperture = 0.1f;
			break;
		case 2:
			world = two_spheres();
			background = color(0.70f, 0.80f, 1.00f);
			lookfrom = point3(13.0f, 2.0f, 3.0f);
			lookat = point3(0.0f, 0.0f, 0.0f);
			vfov = 20.0f;
			break;
		case 3:
			world = two_perlin_spheres();
			background = color(0.70f, 0.80f, 1.00f);
			lookfrom = point3(13.0f, 2.0f, 3.0f);
			lookat = point3(0.0f, 0.0f, 0.0f);
			vfov = 20.0f;
			break;
		case 4:
			//world = earth();
			background = color(0.70f, 0.80f, 1.00f);
			lookfrom = point3(13.0f, 2.0f, 3.0f);
			lookat = point3(0.0f, 0.0f, 0.0f);
			vfov = 20.0f;
			break;

		default:
		case 5:
			world = simple_light();
			background = color(0.0f, 0.0f, 0.0f);
			lookfrom = point3(26.0f, 3.0f, 6.0f);
			lookat = point3(0.0f, 2.0f, 0.0f);
			vfov = 20.0f;
			break;
		}

		// Camera

		vec3 vup(0.0f, 1.0f, 0.0f);
		auto dist_to_focus = 10.0f;
		//int image_height = static_cast<int>(image_width / aspect_ratio);
		//image_height;

		rcCamera cam(lookfrom, lookat, vup, vfov, aspect_ratio, aperture, dist_to_focus);

	//	hittable_list world;

	//	auto material_ground = make_shared<lambertian>(color(0.8f, 0.8f, 0.0f));
	//	auto material_center = make_shared<lambertian>(color(0.1f, 0.2f, 0.5f));
	//	auto material_left = make_shared<dielectric>(1.5f);
	//	auto material_right = make_shared<metal>(color(0.8f, 0.6f, 0.2f), 0.0f);

	//	auto checker = make_shared<checker_texture>(color(0.2f, 0.3f, 0.1f), color(0.9f, 0.9f, 0.9f));
	//	world.add(make_shared<sphere>(point3(0.0f, -1000.0f, 0.0f), 1000.0f, make_shared<lambertian>(checker)));

	//	//world.add(make_shared<sphere>(point3(0.0f, -100.5f, -1.0f), 100.0f, material_ground));
	//	world.add(make_shared<sphere>(point3(0.0f, 0.0f, -1.0f), 0.5f, material_center));
	//	//world.add(make_shared<sphere>(point3(-1.0f, 0.0f, -1.0f), 0.5f, material_left));
	//	world.add(make_shared<sphere>(point3(-1.0f, 0.0f, -1.0f), -0.45f, material_left));
	//	world.add(make_shared<sphere>(point3(1.0f, 0.0f, -1.0f), 0.5f, material_right));
	//	
	//	
	////	auto world = random_scene();

	//	point3 lookfrom(13.0f, 2.0f, 3.0f);
	//	point3 lookat(0.0f, 0.0f, -1.0f);
	//	vec3 vup(0.0f, 1.0f, 0.0f);
	//	auto dist_to_focus = 10.0f;
	//	auto aperture = 0.1f;

	///*	point3 lookfrom(3.0f, 3.0f, 2.0f);
	//	point3 lookat(0.0f, 0.0f, -1.0f);
	//	vec3 vup(0.0f, 1.0f, 0.0f);
	//	auto dist_to_focus = (lookfrom - lookat).length();
	//	auto aperture = 2.0f;*/

	//	rcCamera cam(lookfrom, lookat, vup, 20.0f, aspect_ratio, aperture, dist_to_focus);
	

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
					 
					 vec3 c = ray_color(r, background, world, max_depth);
					
					pixel_color += c;
				}
				WritePixelToBuffer(buffer, i, j, samples_per_pix, pixel_color);


			}
		jCounter--;
	
	}

	
	//color CpuWriteToTextureDemo::ray_color(const Hybrid::ray& r, const Hybrid::hittable& world, int depth)
	//{
	//	
	//	hit_record rec;
	//	// If we've exceeded the ray bounce limit, no more light is gathered.
	//	if (depth <= 0)
	//		return color(0, 0, 0);

	//	if (world.hit(r, 0.001f, infinity, rec)) {
	//		ray scattered;
	//		color attenuation;
	//		if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
	//			return attenuation * ray_color(scattered, world, depth - 1);
	//		return color(0, 0, 0);
	//	}

	//	vec3 unit_direction = unit_vector(r.direction());
	//	float t = 0.5f * (unit_direction.y() + 1.0f);
	//	color toReturn = (1.0f - t) * color(1.0f, 1.0f, 1.0f) + t * color(0.5f, 0.7f, 1.0f);
	//	return toReturn;
	//		
	//}

	color CpuWriteToTextureDemo::ray_color(const ray& r, const color& background, const hittable& world, int depth) {
		hit_record rec;

		// If we've exceeded the ray bounce limit, no more light is gathered.
		if (depth <= 0)
			return color(0.0f, 0.0f, 0.0f);

		// If the ray hits nothing, return the background color.
		if (!world.hit(r, 0.001f, infinity, rec))
			return background;

		ray scattered;
		color attenuation;
		color emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);

		if (!rec.mat_ptr->scatter(r, rec, attenuation, scattered))
			return emitted;

		return emitted + attenuation * ray_color(scattered, background, world, depth - 1);
	}

	hittable_list CpuWriteToTextureDemo::two_perlin_spheres() {
		hittable_list objects;

		auto pertext = make_shared<noise_texture>(4.0f);
		objects.add(make_shared<sphere>(point3(0.0f, -1000.0f, 0.0f), 1000.0f, make_shared<lambertian>(pertext)));
		objects.add(make_shared<sphere>(point3(0.0f, 2.0f, 0.0f), 2.0f, make_shared<lambertian>(pertext)));

		return objects;
	}

	hittable_list CpuWriteToTextureDemo::simple_light()
	{
		hittable_list objects;

		auto pertext = make_shared<noise_texture>(4.0f);
		objects.add(make_shared<sphere>(point3(0.0f, -1000.0f, 0.0f), 1000.0f, make_shared<lambertian>(pertext)));
		objects.add(make_shared<sphere>(point3(0.0f, 2.0f, 0.0f), 2.0f, make_shared<lambertian>(pertext)));

		auto difflight = make_shared<diffuse_light>(color(4.0f, 4.0f, 4.0f));
		objects.add(make_shared<xy_rect>(3.0f, 5.0f, 1.0f, 3.0f, -2.0f, difflight));

		return objects;
	}

	hittable_list CpuWriteToTextureDemo::two_spheres() {
		hittable_list objects;

		auto checker = make_shared<checker_texture>(color(0.2f, 0.3f, 0.1f), color(0.9f, 0.9f, 0.9f));

		objects.add(make_shared<sphere>(point3(0.0f, -10.0f, 0.0f), 10.0f, make_shared<lambertian>(checker)));
		objects.add(make_shared<sphere>(point3(0.0f, 10.0f, 0.0f), 10.0f, make_shared<lambertian>(checker)));

		return objects;
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


}


	


