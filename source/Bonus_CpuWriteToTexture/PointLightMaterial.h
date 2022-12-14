#pragma once

#include <DirectXColors.h>
#include "../Library.Shared/Material.h"
#include "VectorHelper.h"
#include "MatrixHelper.h"
#include "SamplerStates.h"

namespace Library
{
	class Texture2D;
}

namespace Rendering
{
	class PointLightMaterial : public Library::Material
	{
		RTTI_DECLARATIONS(PointLightMaterial, Library::Material)

	public:
		PointLightMaterial(Library::Game& game, std::shared_ptr<Library::Texture2D> colormap, std::shared_ptr<Library::Texture2D> specularMap);
		PointLightMaterial(const PointLightMaterial&) = default;
		PointLightMaterial& operator=(const PointLightMaterial&) = default;
		PointLightMaterial(PointLightMaterial&&) = default;
		PointLightMaterial& operator=(PointLightMaterial&&) = default;
		virtual ~PointLightMaterial() = default;

		winrt::com_ptr<ID3D11SamplerState> SamplerState() const;
		void SetSamplerState(winrt::com_ptr<ID3D11SamplerState> samplerState);

		std::shared_ptr<Library::Texture2D> ColorMap() const;
		void SetColorMap(std::shared_ptr<Library::Texture2D> texture);

		std::shared_ptr<Library::Texture2D> SpecularMap() const;
		void SetSpecularMap(std::shared_ptr<Library::Texture2D> texture);

		const DirectX::XMFLOAT4& AmbientColor() const;
		void SetAmbientColor(const DirectX::XMFLOAT4& color);

		const DirectX::XMFLOAT3& LightPosition() const;
		void SetLightPosition(const DirectX::XMFLOAT3& position);

		const float LightRadius() const;
		void SetLightRadius(float radius);

		const DirectX::XMFLOAT4& LightColor() const;
		void SetLightColor(const DirectX::XMFLOAT4& color);

		const DirectX::XMFLOAT3& SpecularColor() const;
		void SetSpecularColor(const DirectX::XMFLOAT3& color);

		const float SpecularPower() const;
		void SetSpecularPower(float power);

		virtual std::uint32_t VertexSize() const override;
		virtual void Initialize() override;

		void UpdateCameraPosition(const DirectX::XMFLOAT3& position);
		void UpdateTransforms(DirectX::FXMMATRIX worldViewProjectionMatrix, DirectX::CXMMATRIX worldMatrix);

	private:
		struct VertexCBufferPerFrame
		{
			DirectX::XMFLOAT3 LightPosition{ Library::Vector3Helper::Zero };
			float LightRadius{ 50.0f };
		};

		struct VertexCBufferPerObject
		{
			DirectX::XMFLOAT4X4 WorldViewProjection{ Library::MatrixHelper::Identity };
			DirectX::XMFLOAT4X4 World{ Library::MatrixHelper::Identity };
		};

		struct PixelCBufferPerFrame
		{
			DirectX::XMFLOAT3 CameraPosition{ Library::Vector3Helper::Zero };
			float Padding;
			DirectX::XMFLOAT4 AmbientColor{ DirectX::Colors::Black };
			DirectX::XMFLOAT3 LightPosition{ 0.0f, 0.0f, 0.0f };
			float Padding2;
			DirectX::XMFLOAT4 LightColor{ DirectX::Colors::White };
		};

		struct PixelCBufferPerObject
		{
			DirectX::XMFLOAT3 SpecularColor{ 1.0f, 1.0f, 1.0f };
			float SpecularPower{ 128.0f };
		};

		virtual void BeginDraw() override;

		void ResetPixelShaderResources();

		winrt::com_ptr<ID3D11Buffer> mVertexCBufferPerFrame;
		winrt::com_ptr<ID3D11Buffer> mVertexCBufferPerObject;
		winrt::com_ptr<ID3D11Buffer> mPixelCBufferPerFrame;
		winrt::com_ptr<ID3D11Buffer> mPixelCBufferPerObject;
		VertexCBufferPerFrame mVertexCBufferPerFrameData;
		VertexCBufferPerObject mVertexCBufferPerObjectData;
		PixelCBufferPerFrame mPixelCBufferPerFrameData;
		PixelCBufferPerObject mPixelCBufferPerObjectData;
		bool mVertexCBufferPerFrameDataDirty{ true };
		bool mPixelCBufferPerFrameDataDirty{ true };
		bool mPixelCBufferPerObjectDataDirty{ true };
		std::shared_ptr<Library::Texture2D> mColorMap;
		std::shared_ptr<Library::Texture2D> mSpecularMap;
		winrt::com_ptr<ID3D11SamplerState> mSamplerState{ Library::SamplerStates::TrilinearClamp };
	};
}