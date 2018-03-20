#pragma once
#include "pch.h"
#include "Renderer.h"
#include<DirectXPackedVector.h>

Renderer::Renderer()
{
}


Renderer::~Renderer()
{
}

void Renderer::InitializeD3D(HWND WinHandle, int ScreenWidth, int ScreenHeight)
{
	UNREFERENCED_PARAMETER(WinHandle);
	DXGI_SWAP_CHAIN_DESC SwapChainDescription;

	ZeroMemory(&SwapChainDescription, sizeof(DXGI_SWAP_CHAIN_DESC));

	SwapChainDescription.BufferCount = 1;
	SwapChainDescription.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	SwapChainDescription.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	SwapChainDescription.OutputWindow = WinHandle;
	SwapChainDescription.SampleDesc.Count = 4;
	SwapChainDescription.Windowed = TRUE;
	SwapChainDescription.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	//D3D_FEATURE_LEVEL FeatureLevels[] = { D3D_FEATURE_LEVEL_11_0,D3D_FEATURE_LEVEL_10_1,D3D_FEATURE_LEVEL_10_0 };

	HRESULT Res;
	Res = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, NULL, NULL, D3D11_SDK_VERSION, &SwapChainDescription, &SwapChain, &Device, NULL, &DeviceContext);
	
	ID3D11Texture2D *BackBuffer;
	SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&BackBuffer);

	Device->CreateRenderTargetView(BackBuffer, NULL, &RenderTargetView);
	BackBuffer->Release();

	DeviceContext->OMSetRenderTargets(1, &RenderTargetView, NULL);

	D3D11_VIEWPORT Viewport;
	ZeroMemory(&Viewport, sizeof(Viewport));

	Viewport.TopLeftX = 0;
	Viewport.TopLeftY = 0;
	Viewport.Width = (float)ScreenWidth;
	Viewport.Height = (float)ScreenHeight;

	DeviceContext->RSSetViewports(1, &Viewport);

	
}

void Renderer::ShutDownD3D()
{	
	Device->Release();
	DeviceContext->Release();
	SwapChain->Release();
}

void Renderer::RenderFrame()
{
	float Color[4] = { 0.0f,0.2f,0.4f,1.0f };
	DeviceContext->ClearRenderTargetView(RenderTargetView, Color);
	SwapChain->Present(0, 0);
	RenderTargetView->Release();
}