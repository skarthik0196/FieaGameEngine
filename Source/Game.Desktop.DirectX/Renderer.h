#pragma once
#include<Windows.h>
#include "RendererCore.h"


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

	private:
		uint32_t MipLevels;

		std::vector<Microsoft::WRL::ComPtr<ID3D11RenderTargetView>> RenderTargetViews;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> DepthStencilView;
	};
}