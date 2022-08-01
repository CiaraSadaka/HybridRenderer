//#include "pch.h"
//#include "RastMode.h"
//#include "Utility.h"
//#include "ColorHelper.h"
//#include "Camera.h"
//#include "VertexDeclarations.h"
//#include "Game.h"
//#include "GameException.h"
//#include "Model.h"
//#include "Mesh.h"
//#include "SamplerStates.h"

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
	RastMode::RastMode(Game& game, const shared_ptr<Camera>& camera) :
		DrawableGameComponent(game, camera)
	{
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
		const auto model = mGame->Content().Load<Library::Model>(L"Models\\Sphere.obj.bin"s);
		Mesh* mesh = model->Meshes().at(0).get();
		VertexPositionTextureNormal::CreateVertexBuffer(direct3DDevice, *mesh, not_null<ID3D11Buffer**>(mVertexBuffer.put()));
		mesh->CreateIndexBuffer(*direct3DDevice, not_null<ID3D11Buffer**>(mIndexBuffer.put()));
		mIndexCount = narrow<uint32_t>(mesh->Indices().size());

		auto colorMap = mGame->Content().Load<Texture2D>(L"Textures\\EarthComposite.dds"s);
		auto specularMap = mGame->Content().Load<Texture2D>(L"Textures\\EarthComposite.dds"s);
		mMaterial = make_shared<PointLightMaterial>(*mGame, colorMap, specularMap);
		mMaterial->Initialize();

		mProxyModel = make_unique<ProxyModel>(*mGame, mCamera, "Models\\PointLightProxy.obj.bin"s, 0.5f);
		mProxyModel->Initialize();

		SetLightPosition(XMFLOAT3(1.0f, 0.0, 8.0f));

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
	}


	void RastMode::Update(const GameTime& gameTime)
	{
		if (mAnimationEnabled)
		{
			mModelRotationAngle += gameTime.ElapsedGameTimeSeconds().count() * RotationRate;
			XMStoreFloat4x4(&mWorldMatrix, XMMatrixRotationY(mModelRotationAngle));
			mUpdateMaterial = true;
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

	//void RastMode::CreateVertexBuffer(const Mesh& mesh, not_null<ID3D11Buffer**> vertexBuffer) const
	//{
	//	const vector<XMFLOAT3>& sourceVertices = mesh.Vertices();

	//	vector<VertexPositionColor> vertices;
	//	vertices.reserve(sourceVertices.size());
	//	if (mesh.VertexColors().size() > 0)
	//	{
	//		const vector<XMFLOAT4>& vertexColors = mesh.VertexColors().at(0);
	//		assert(vertexColors.size() == sourceVertices.size());

	//		for (size_t i = 0; i < sourceVertices.size(); i++)
	//		{
	//			const XMFLOAT3& position = sourceVertices.at(i);
	//			const XMFLOAT4& color = vertexColors.at(i);
	//			vertices.emplace_back(XMFLOAT4(position.x, position.y, position.z, 1.0f), color);
	//		}
	//	}
	//	else
	//	{
	//		for (size_t i = 0; i < sourceVertices.size(); i++)
	//		{
	//			const XMFLOAT3& position = sourceVertices.at(i);
	//			XMFLOAT4 color = ColorHelper::RandomColor();
	//			vertices.emplace_back(XMFLOAT4(position.x, position.y, position.z, 1.0f), color);
	//		}
	//	}

	//	D3D11_BUFFER_DESC vertexBufferDesc{ 0 };
	//	vertexBufferDesc.ByteWidth = VertexPositionColor::VertexBufferByteWidth(vertices.size());
	//	vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	//	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	//	D3D11_SUBRESOURCE_DATA vertexSubResourceData{ 0 };
	//	vertexSubResourceData.pSysMem = &vertices[0];
	//	ThrowIfFailed(mGame->Direct3DDevice()->CreateBuffer(&vertexBufferDesc, &vertexSubResourceData, vertexBuffer), "ID3D11Device::CreateBuffer() failed.");
	//}
	/*void RastMode::CreateVertexBuffer(const Mesh& mesh, not_null<ID3D11Buffer**> vertexBuffer) const
	{
		const vector<XMFLOAT3>& sourceVertices = mesh.Vertices();
		const auto& sourceUVs = mesh.TextureCoordinates().at(0);

		vector<VertexPositionTexture> vertices;
		vertices.reserve(sourceVertices.size());
		for (size_t i = 0; i < sourceVertices.size(); i++)
		{
			const XMFLOAT3& position = sourceVertices.at(i);
			const XMFLOAT3& uv = sourceUVs.at(i);
			vertices.emplace_back(XMFLOAT4(position.x, position.y, position.z, 1.0f), XMFLOAT2(uv.x, uv.y));
		}

		D3D11_BUFFER_DESC vertexBufferDesc{ 0 };
		vertexBufferDesc.ByteWidth = VertexPositionTexture::VertexBufferByteWidth(vertices.size());
		vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA vertexSubResourceData{ 0 };
		vertexSubResourceData.pSysMem = &vertices[0];
		ThrowIfFailed(mGame->Direct3DDevice()->CreateBuffer(&vertexBufferDesc, &vertexSubResourceData, vertexBuffer), "ID3D11Device::CreateBuffer() failed.");
	}*/
}