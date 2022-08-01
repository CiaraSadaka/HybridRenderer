#pragma once

#include <gsl\gsl>
#include <winrt\Windows.Foundation.h>
#include <d3d11.h>
#include "DrawableGameComponent.h"
#include "MatrixHelper.h"
#include "PointLight.h"

namespace Library
{
	class ProxyModel;
}

namespace Rendering
{
	class PointLightMaterial;

	class RastMode final : public Library::DrawableGameComponent
	{
	public:
		RastMode(Library::Game& game, const std::shared_ptr<Library::Camera>& camera, float orbitalDist, std::wstring texName, float scale, float ambientLight, float axialTilt, float rotationalPeriod, float orbitalPeriod, std::shared_ptr<RastMode> parent);
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
		void SetLightPosition(const DirectX::XMFLOAT3& position);
		void SetLightPosition(DirectX::FXMVECTOR position);

		float LightRadius() const;
		void SetLightRadius(float radius);

		float SpecularIntensity() const;
		void SetSpecularIntensity(float intensity);

		float SpecularPower() const;
		void SetSpecularPower(float power);

		virtual void Initialize() override;
		virtual void Update(const Library::GameTime& gameTime) override;
		virtual void Draw(const Library::GameTime& gameTime) override;

	private:
		inline static const float RotationRate{ DirectX::XM_PI };

		std::shared_ptr<PointLightMaterial> mMaterial;
		DirectX::XMFLOAT4X4 mWorldMatrix{ Library::MatrixHelper::Identity };
		winrt::com_ptr<ID3D11Buffer> mVertexBuffer;
		winrt::com_ptr<ID3D11Buffer> mIndexBuffer;
		std::uint32_t mIndexCount{ 0 };
		Library::PointLight mPointLight;
		std::unique_ptr<Library::ProxyModel> mProxyModel;
		float _x{0.0f};
		float _y{ 0.0f };
		float _z{ 0.0f };
		float mModelRotationAngle{ 0.0f };
		bool mAnimationEnabled{ true };
		bool mUpdateMaterial{ true };

		float mOrbitalDistance{ 0.0f };
		std::wstring mTexName;
		float mScale{ 0.0f };
		float mAmbientLight{ 0.0f };
		float mAxialTilt{ 0.0f };
		float mRotationalPeriod{ 0.0f };
		float mOrbitalPeriod{ 0.0f };
		std::shared_ptr<RastMode> mParent;
		const inline static float sOrbitalRate = 0.2175f;
		const inline static float sRotationRate = sOrbitalRate * 6.374f;

		DirectX::XMFLOAT4X4 mScaleMatrix{ Library::MatrixHelper::Identity };

		float mModelOrbitalAngle{ 0.0f };
	};
}