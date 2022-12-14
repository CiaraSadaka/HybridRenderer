#include "pch.h"
#include "RenderingGame.h"
#include "PerspectiveCamera.h"
#include "GameException.h"
#include "KeyboardComponent.h"
#include "FpsComponent.h"
#include "CpuWriteToTextureDemo.h"
#include "RastMode.h"
#include "FirstPersonCamera.h"
#include "SamplerStates.h"
#include "RasterizerStates.h"
#include "VectorHelper.h"
#include "ImGuiComponent.h"
#include "imgui_impl_dx11.h"
#include "UtilityWin32.h"
#include "Grid.h"

using namespace std;
using namespace DirectX;
using namespace Library;

namespace Rendering
{
	RenderingGame::RenderingGame(std::function<void*()> getWindowCallback, std::function<void(SIZE&)> getRenderTargetSizeCallback) :
		Game(getWindowCallback, getRenderTargetSizeCallback)
	{
	}

	void RenderingGame::Initialize()
	{
		auto direct3DDevice = Direct3DDevice();
		SamplerStates::Initialize(direct3DDevice); 
		RasterizerStates::Initialize(direct3DDevice);

		mKeyboard = make_shared<KeyboardComponent>(*this);
		mComponents.push_back(mKeyboard);
		mServices.AddService(KeyboardComponent::TypeIdClass(), mKeyboard.get());
	
		/*auto camera = make_shared<PerspectiveCamera>(*this);
		mComponents.push_back(camera);
		mServices.AddService(Camera::TypeIdClass(), camera.get());*/

		
		auto camera = make_shared<FirstPersonCamera>(*this);
		mComponents.push_back(camera);
		mServices.AddService(Camera::TypeIdClass(), camera.get());


		mGrid = make_shared<Grid>(*this, camera);
		mComponents.push_back(mGrid);


		//mImGuiComponent = make_shared<ImGuiComponent>(*this);
		//mServices.AddService(ImGuiComponent::TypeIdClass(), mImGuiComponent.get());
		//auto imGuiWndProcHandler = make_shared<UtilityWin32::WndProcHandler>(ImGui_ImplWin32_WndProcHandler);
		//UtilityWin32::AddWndProcHandler(imGuiWndProcHandler);

		//auto helpTextImGuiRenderBlock = make_shared<ImGuiComponent::RenderBlock>([this]()
		//{
		//	ImGui::Begin("Controls");
		//	ImGui::SetNextWindowPos(ImVec2(10, 10));
		//	
		//	{
		//		stringstream fpsLabel;
		//		fpsLabel << setprecision(3) << "Frame Rate: " << mFpsComponent->FrameRate() << "    Total Elapsed Time: " << mGameTime.TotalGameTimeSeconds().count();
		//		ImGui::Text(fpsLabel.str().c_str());
		//	}
		//
		//	ImGui::End();
		//});
		//mImGuiComponent->AddRenderBlock(helpTextImGuiRenderBlock);
		//mImGuiComponent->Initialize();

	
		mCpuWriteToTextureDemo = make_shared<CpuWriteToTextureDemo>(*this, camera);
		mComponents.push_back(mCpuWriteToTextureDemo);

		mCpuWriteToTextureDemo->SetEnabled(0);
		mCpuWriteToTextureDemo->SetVisible(0);

	/*	mRastMode = make_shared<RastMode>(*this, camera, 0.0f, 0.0f, 0.0f);
		mComponents.push_back(mRastMode);*/
		mEarth = make_shared<RastMode>(*this, camera, 0.0f, L"Textures\\EarthComposite.dds"s, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, nullptr, 1.0f, 1.0f, 1.0f);
		mComponents.push_back(mEarth);

		mEarth2 = make_shared<RastMode>(*this, camera, 0.0f, L"Textures\\EarthComposite.dds"s, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, nullptr, 10.0f, 1.0f, 1.0f);
		mComponents.push_back(mEarth2);

		auto imGui = make_shared<ImGuiComponent>(*this);
		mComponents.push_back(imGui);
		mServices.AddService(ImGuiComponent::TypeIdClass(), imGui.get());
		auto imGuiWndProcHandler = make_shared<UtilityWin32::WndProcHandler>(ImGui_ImplWin32_WndProcHandler);
		UtilityWin32::AddWndProcHandler(imGuiWndProcHandler);

		auto helpTextImGuiRenderBlock = make_shared<ImGuiComponent::RenderBlock>([this]()
			{
				ImGui::Begin("Controls");
				ImGui::SetNextWindowPos(ImVec2(10, 10));

				{
					stringstream fpsLabel;
					fpsLabel << setprecision(3) << "Frame Rate: " << mFpsComponent->FrameRate() << "    Total Elapsed Time: " << mGameTime.TotalGameTimeSeconds().count();
					ImGui::Text(fpsLabel.str().c_str());
				}

				ImGui::Text("Camera (WASD + Left-Click-Mouse-Look)");
				ImGui::Text("Move Point Light (Num-Pad 8/2, 4/6, 3/9)");

				{
					stringstream gridVisibleLabel;
					gridVisibleLabel << "Toggle Grid (G): " << (mGrid->Visible() ? "Visible" : "Not Visible");
					ImGui::Text(gridVisibleLabel.str().c_str());
				}


				ImGui::End();
			});
		imGui->AddRenderBlock(helpTextImGuiRenderBlock);

		mFpsComponent = make_shared<FpsComponent>(*this);
		mFpsComponent->SetVisible(false);
		mComponents.push_back(mFpsComponent);


		Game::Initialize();


		camera->SetPosition(0.0f, 2.5f, 20.0f);

	//	mAmbientLightIntensity = mRastMode->AmbientLightIntensity();
	/*	mPointLightIntensity = mRastMode->PointLightIntensity();
		mSpecularIntensity = mRastMode->SpecularIntensity();
		mSpecularPower = mRastMode->SpecularPower();*/
	//	mCpuWriteToTextureDemo = std::make_shared<CpuWriteToTextureDemo>(mGame, camera);
	}

	void RenderingGame::Update(const GameTime &gameTime)
	{
		if (mKeyboard->WasKeyPressedThisFrame(Keys::Escape))
		{
			Exit();
		}
		if (mKeyboard->WasKeyPressedThisFrame(Keys::Space))
		{
	
			mCpuWriteToTextureDemo->SetEnabled(!mCpuWriteToTextureDemo->Enabled());
			mCpuWriteToTextureDemo->SetVisible(mCpuWriteToTextureDemo->Enabled());

			mEarth->SetEnabled(!mEarth->Enabled());
			mEarth->SetVisible(mEarth->Enabled());
			mEarth2->SetEnabled(!mEarth->Enabled());
			mEarth2->SetVisible(mEarth->Enabled());
		
		}
	//	mImGuiComponent->Update(gameTime);

		UpdateAmbientLightIntensity(gameTime);


		Game::Update(gameTime);

	}

	void RenderingGame::Draw(const GameTime &gameTime)
	{		
		mDirect3DDeviceContext->ClearRenderTargetView(mRenderTargetView.get(), BackgroundColor.f);
		mDirect3DDeviceContext->ClearDepthStencilView(mDepthStencilView.get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);



		Game::Draw(gameTime);

	//	mImGuiComponent->Draw(gameTime);

		HRESULT hr = mSwapChain->Present(1, 0);

		// If the device was removed either by a disconnection or a driver upgrade, we must recreate all device resources.
		if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
		{
			HandleDeviceLost();
		}
		else
		{
			ThrowIfFailed(hr, "IDXGISwapChain::Present() failed.");
		}
	}

	void RenderingGame::Shutdown()
	{
		mFpsComponent = nullptr;
	//	mImGuiComponent->Shutdown();
	//	mImGuiComponent = nullptr;
		mCpuWriteToTextureDemo = nullptr;
		mRastMode = nullptr;
		RasterizerStates::Shutdown();
		SamplerStates::Shutdown();
		Game::Shutdown();		
	}


	void RenderingGame::UpdateAmbientLightIntensity(const GameTime& gameTime)
	{
		if (mKeyboard->IsKeyDown(Keys::PageUp) && mAmbientLightIntensity < 1.0f)
		{
			mAmbientLightIntensity += gameTime.ElapsedGameTimeSeconds().count();
			mAmbientLightIntensity = min(mAmbientLightIntensity, 1.0f);
		}
		else if (mKeyboard->IsKeyDown(Keys::PageDown) && mAmbientLightIntensity > 0.0f)
		{
			mAmbientLightIntensity -= gameTime.ElapsedGameTimeSeconds().count();
			mAmbientLightIntensity = max(mAmbientLightIntensity, 0.0f);
		}
	}



	void RenderingGame::Exit()
	{
		PostQuitMessage(0);
	}
}