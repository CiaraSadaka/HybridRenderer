#pragma once

#include <gsl\gsl>
#include <winrt\Windows.Foundation.h>
#include <d3d11.h>
#include "DrawableGameComponent.h"
#include "MatrixHelper.h"
#include "PointLight.h"

namespace Library
{
	class Mesh;
}

namespace Rendering
{
	
	class RastMode final : public Library::DrawableGameComponent
	{
	public:
		RastMode(Library::Game& game, const std::shared_ptr<Library::Camera>& camera);
		RastMode(const RastMode&) = delete;
		RastMode(RastMode&&) = default;
		RastMode& operator=(const RastMode&) = default;
		RastMode& operator=(RastMode&&) = default;
		~RastMode();

		bool AnimationEnabled() const;
		void SetAnimationEnabled(bool enabled);
		void ToggleAnimation();

		float AmbientLightIntensity() const;
		void SetAmbientLightIntensity(float intensity);

		float PointLightIntensity() const;
		void SetPointLightIntensity(float intensity);

		const DirectX::XMFLOAT3& LightPosition() const;
		const DirectX::XMVECTOR LightPositionVector() const;
		void SetLightPosition(const DirectX::XMFLOAT3 & position);
		void SetLightPosition(DirectX::FXMVECTOR position);

		float LightRadius() const;
		void SetLightRadius(float radius);

		float SpecularIntensity() const;
		void SetSpecularIntensity(float intensity);

		float SpecularPower() const;
		void SetSpecularPower(float power);

		virtual void Initialize() override;
		virtual void Update(const Library::GameTime & gameTime) override;
		virtual void Draw(const Library::GameTime & gameTime) override;
	private:
		//struct CBufferPerObject
		//{
		////	DirectX::XMFLOAT4X4 WorldViewProjection;
		//	DirectX::XMFLOAT4X4 WorldViewProjection{ Library::MatrixHelper::Identity };
		//};

		//void CreateVertexBuffer(const Library::Mesh& mesh, gsl::not_null<ID3D11Buffer**> vertexBuffer) const;
		////void CreateVertexBuffer(ID3D11Device* device, const Mesh& mesh, ID3D11Buffer** vertexBuffer) const;


		//inline static const float RotationRate{ DirectX::XM_PI };

		//DirectX::XMFLOAT4X4 mWorldMatrix{ Library::MatrixHelper::Identity };
		//CBufferPerObject mCBufferPerObject;
		//winrt::com_ptr<ID3D11VertexShader> mVertexShader;
		//winrt::com_ptr<ID3D11PixelShader> mPixelShader;
		//winrt::com_ptr<ID3D11InputLayout> mInputLayout;
		//winrt::com_ptr<ID3D11Buffer> mVertexBuffer;
		//winrt::com_ptr<ID3D11Buffer> mIndexBuffer;
		//winrt::com_ptr<ID3D11Buffer> mConstantBuffer;
		//winrt::com_ptr<ID3D11ShaderResourceView> mColorTexture;
		//std::uint32_t mIndexCount{ 0 };
		//float mRotationAngle{ 0.0f };
		//bool mAnimationEnabled{ false };
		//bool mUpdateConstantBuffer{ true };
		inline static const float RotationRate{ DirectX::XM_PI };

		std::shared_ptr<PointLightMaterial> mMaterial;
		DirectX::XMFLOAT4X4 mWorldMatrix{ Library::MatrixHelper::Identity };
		winrt::com_ptr<ID3D11Buffer> mVertexBuffer;
		winrt::com_ptr<ID3D11Buffer> mIndexBuffer;
		std::uint32_t mIndexCount{ 0 };
		Library::PointLight mPointLight;
		std::unique_ptr<Library::ProxyModel> mProxyModel;
		float mModelRotationAngle{ 0.0f };
		bool mAnimationEnabled{ true };
		bool mUpdateMaterial{ true };


	};
}