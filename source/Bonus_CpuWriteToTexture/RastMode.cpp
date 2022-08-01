#include "pch.h"
#include "RastMode.h"
#include "FirstPersonCamera.h"
#include "VertexDeclarations.h"
#include "Game.h"
#include "GameException.h"
#include "Model.h"
#include "Mesh.h"
#include "ProxyModel.h"
#include "PointLightMaterial.h"
#include "Texture2D.h"

using namespace std;
using namespace std::string_literals;
using namespace gsl;
using namespace Library;
using namespace DirectX;

namespace Rendering
{
	RastMode::RastMode(Library::Game& game, const std::shared_ptr<Library::Camera>& camera, float orbitalDist, std::wstring texName, float scale, float ambientLight, float axialTilt, float rotationalPeriod, float orbitalPeriod, std::shared_ptr<RastMode> parent, float x, float y, float z) :
		DrawableGameComponent(game, camera), mTexName(texName), mScale(scale), mAxialTilt(axialTilt), mRotationalPeriod(rotationalPeriod), mOrbitalPeriod(orbitalPeriod), mParent(parent), _x(x), _y(y), _z(z)
 	{
		mScale = scale;
		mAmbientLight = ambientLight;
		mOrbitalDistance = orbitalDist;
	}


	RastMode::~RastMode()
	{
	}

	bool RastMode::AnimationEnabled() const
	{
		return mAnimationEnabled;
	}

	void RastMode::SetAnimationEnabled(bool enabled)
	{
		mAnimationEnabled = enabled;
	}

	void RastMode::ToggleAnimation()
	{
		mAnimationEnabled = !mAnimationEnabled;
	}

	float RastMode::AmbientLightIntensity() const
	{
		return mMaterial->AmbientColor().x;
	}

	void RastMode::SetAmbientLightIntensity(float intensity)
	{
		mMaterial->SetAmbientColor(XMFLOAT4(intensity, intensity, intensity, 1.0f));
	}

	float RastMode::PointLightIntensity() const
	{
		return mMaterial->LightColor().x;
	}

	void RastMode::SetPointLightIntensity(float intensity)
	{
		mMaterial->SetLightColor(XMFLOAT4(intensity, intensity, intensity, 1.0f));
	}

	const XMFLOAT3& RastMode::LightPosition() const
	{
		return mPointLight.Position();
	}

	const XMVECTOR RastMode::LightPositionVector() const
	{
		return mPointLight.PositionVector();
	}

	void RastMode::SetLightPosition(const XMFLOAT3& position)
	{
		mPointLight.SetPosition(position);
		mProxyModel->SetPosition(position);
		mMaterial->SetLightPosition(position);
	}

	void RastMode::SetLightPosition(FXMVECTOR position)
	{
		mPointLight.SetPosition(position);
		mProxyModel->SetPosition(position);

		XMFLOAT3 materialPosition;
		XMStoreFloat3(&materialPosition, position);
		mMaterial->SetLightPosition(materialPosition);
	}

	float RastMode::LightRadius() const
	{
		return mMaterial->LightRadius();
	}

	void RastMode::SetLightRadius(float radius)
	{
		mMaterial->SetLightRadius(radius);
	}

	float RastMode::SpecularIntensity() const
	{
		return mMaterial->SpecularColor().x;
	}

	void RastMode::SetSpecularIntensity(float intensity)
	{
		mMaterial->SetSpecularColor(XMFLOAT3(intensity, intensity, intensity));
	}

	float RastMode::SpecularPower() const
	{
		return mMaterial->SpecularPower();
	}

	void RastMode::SetSpecularPower(float power)
	{
		mMaterial->SetSpecularPower(power);
	}

	void RastMode::Initialize()
	{
		auto direct3DDevice = mGame->Direct3DDevice();
		const auto model = mGame->Content().Load<Model>(L"Models\\Sphere.obj.bin"s);
		Mesh* mesh = model->Meshes().at(0).get();
		VertexPositionTextureNormal::CreateVertexBuffer(direct3DDevice, *mesh, not_null<ID3D11Buffer**>(mVertexBuffer.put()));
		mesh->CreateIndexBuffer(*direct3DDevice, not_null<ID3D11Buffer**>(mIndexBuffer.put()));
		mIndexCount = narrow<uint32_t>(mesh->Indices().size());

		auto colorMap = mGame->Content().Load<Texture2D>(mTexName);
		auto specularMap = mGame->Content().Load<Texture2D>(mTexName);
		mMaterial = make_shared<PointLightMaterial>(*mGame, colorMap, specularMap);
		mMaterial->Initialize();

		mProxyModel = make_unique<ProxyModel>(*mGame, mCamera, "Models\\PointLightProxy.obj.bin"s, 0.5f);
		mProxyModel->Initialize();

		SetLightPosition(XMFLOAT3(0.0f, 0.0, 8.0f));

		mMaterial->SetAmbientColor(DirectX::XMFLOAT4(mAmbientLight, mAmbientLight, mAmbientLight, mAmbientLight));
		mMaterial->SetLightRadius(10000.0f);
		mMaterial->SetSpecularColor(XMFLOAT3(0.f, 0.f, 0.f));

		auto updateMaterialFunc = [this]() { mUpdateMaterial = true; };
		mCamera->AddViewMatrixUpdatedCallback(updateMaterialFunc);
		mCamera->AddProjectionMatrixUpdatedCallback(updateMaterialFunc);

		auto firstPersonCamera = mCamera->As<FirstPersonCamera>();
		if (firstPersonCamera != nullptr)
		{
			firstPersonCamera->AddPositionUpdatedCallback([this]() {
				mMaterial->UpdateCameraPosition(mCamera->Position());
				});
		}

		XMStoreFloat4x4(&mScaleMatrix, XMMatrixMultiply(XMLoadFloat4x4(&mWorldMatrix), XMMatrixScaling(mScale, mScale, mScale)));
		XMStoreFloat4x4(&mWorldMatrix, XMMatrixMultiply(XMLoadFloat4x4(&mWorldMatrix), XMMatrixTranslation(_x, _y, _z)));
		XMStoreFloat4x4(&mWorldMatrix, XMMatrixMultiply(XMLoadFloat4x4(&mWorldMatrix), XMMatrixRotationY(mModelOrbitalAngle)));
	}

	void RastMode::Update(const GameTime& gameTime)
	{
		if (mAnimationEnabled)
		{



			mUpdateMaterial = true;
		}
		if (mParent)
		{
			XMStoreFloat4x4(&mWorldMatrix, XMMatrixMultiply(XMLoadFloat4x4(&mWorldMatrix), XMMatrixTranslation(mParent->mOrbitalDistance * 100.0f, 0, 0)));
			XMStoreFloat4x4(&mWorldMatrix, XMMatrixMultiply(XMLoadFloat4x4(&mWorldMatrix), XMMatrixRotationY(mParent->mModelOrbitalAngle)));
		}

		mProxyModel->Update(gameTime);
	}

	void RastMode::Draw(const GameTime& gameTime)
	{

		if (mUpdateMaterial)
		{
			const XMMATRIX worldMatrix = XMLoadFloat4x4(&mWorldMatrix);
			const XMMATRIX wvp = XMMatrixTranspose(worldMatrix * mCamera->ViewProjectionMatrix());
			mMaterial->UpdateTransforms(wvp, XMMatrixTranspose(worldMatrix));
			mUpdateMaterial = false;
		}

		mMaterial->DrawIndexed(not_null<ID3D11Buffer*>(mVertexBuffer.get()), not_null<ID3D11Buffer*>(mIndexBuffer.get()), mIndexCount);
		mProxyModel->Draw(gameTime);
	}
}