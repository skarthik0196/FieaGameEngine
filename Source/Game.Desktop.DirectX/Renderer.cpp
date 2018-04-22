#pragma once
#include "pch.h"
#include "Renderer.h"
#include<DirectXPackedVector.h>

namespace Rendering
{
	Renderer::Renderer() : RendererCore()
	{
	}

	Renderer::Renderer(HWND windowHandle, int screenWidth, int screenHeight) : RendererCore(windowHandle, screenWidth, screenHeight)
	{
	}

	Renderer::Renderer(HWND windowHandle) : RendererCore(windowHandle)
	{
	}

	Renderer::~Renderer()
	{
	}
	void Renderer::InitializeRenderer()
	{
		HRESULT result;
		RendererCore::InitializeD3DCore();

		ID3D11Texture2D *backBuffer;
		result = GetSwapChain()->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));

		D3D11_TEXTURE2D_DESC depthStencilBufferDescription;
		ZeroMemory(&depthStencilBufferDescription, sizeof(depthStencilBufferDescription));

		depthStencilBufferDescription.Width = GetScreenWidth();
		depthStencilBufferDescription.Height = GetScreenHeight();
		depthStencilBufferDescription.MipLevels = 1;
		depthStencilBufferDescription.ArraySize = 1;
		depthStencilBufferDescription.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilBufferDescription.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthStencilBufferDescription.Usage = D3D11_USAGE_DEFAULT;
		depthStencilBufferDescription.SampleDesc.Count = GetMultiSamplingCount();
		depthStencilBufferDescription.SampleDesc.Quality = GetMultiSamplingQualityLevels() - 1;

		ID3D11Texture2D *depthStencilBuffer;
		result = GetDevice()->CreateTexture2D(&depthStencilBufferDescription, nullptr, &depthStencilBuffer);
		result = GetDevice()->CreateDepthStencilView(depthStencilBuffer, nullptr, DepthStencilView.GetAddressOf());

		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView;

		result = GetDevice()->CreateRenderTargetView(backBuffer, nullptr, renderTargetView.GetAddressOf());
		RenderTargetViews.push_back(renderTargetView);

		GetDeviceContext()->OMSetRenderTargets(RenderTargetViews.size(), RenderTargetViews[0].GetAddressOf(), DepthStencilView.Get());
		CreateViewPort();
	}
	void Renderer::CreateViewPort()
	{
		D3D11_VIEWPORT viewPortDescription;
		ZeroMemory(&viewPortDescription, sizeof(viewPortDescription));

		viewPortDescription.TopLeftX = 0;
		viewPortDescription.TopLeftY = 0;
		viewPortDescription.Width = static_cast<float>(GetScreenWidth());
		viewPortDescription.Height = static_cast<float>(GetScreenHeight());

		GetDeviceContext()->RSSetViewports(1, &viewPortDescription);
	}

	void Renderer::RenderFrame()
	{
		ID3D11DeviceContext2* deviceContext = GetDeviceContext();
		static DirectX::XMVECTORF32 BGColor = { 0.0f,0.2f,0.4f,1.0f };

		deviceContext->ClearRenderTargetView(RenderTargetViews[0].Get(), reinterpret_cast<float*>(&BGColor));
		deviceContext->ClearDepthStencilView(DepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		GetSwapChain()->Present(0,0);

	}
}