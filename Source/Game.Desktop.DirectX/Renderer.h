#pragma once
#include<Windows.h>
#include "RendererCore.h"
#include "CameraBase.h"

namespace Rendering
{
	class Renderer : public RendererCore
	{
	public:
		Renderer();
		Renderer(HWND windowHandle, int screenWidth, int screenHeight);
		Renderer(HWND windowHandle);
		Renderer(const Renderer& rhs) = default;
		Renderer(Renderer&& rhs) = default;
		virtual ~Renderer();

		void InitializeRenderer();

		void CreateViewPort();

		void RenderFrame();

		std::shared_ptr<CameraBase> GetMainCamera();

	private:
		void SetCameraResolution();

		uint32_t MipLevels;

		std::vector<Microsoft::WRL::ComPtr<ID3D11RenderTargetView>> RenderTargetViews;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> DepthStencilView;

		std::shared_ptr<CameraBase> MainCamera;
	};
}