#pragma once
#include<Windows.h>
#include<d3d11.h>



class Renderer
{
public:
	Renderer();
	~Renderer();

	void InitializeD3D(HWND WindowHandle, int ScreenWidth, int ScreenHeight);
	void ShutDownD3D();
	void RenderFrame();

private:
	IDXGISwapChain *SwapChain;
	ID3D11Device *Device;
	ID3D11DeviceContext *DeviceContext;
	ID3D11RenderTargetView *RenderTargetView;
};

