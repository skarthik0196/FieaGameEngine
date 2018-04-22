#pragma once
#include <d3d11_2.h>
#include <memory>
#include <wrl.h>

namespace Rendering
{
	class RendererCore
	{
	public:
		RendererCore();
		RendererCore(HWND windowHandle, uint32_t screenWidth, uint32_t screenHeight);
		RendererCore(HWND windowHandle);
		RendererCore(const RendererCore& rhs) = default;
		RendererCore(RendererCore&& rhs) = default;
		virtual ~RendererCore();

		RendererCore& operator=(const RendererCore& rhs) = default;
		RendererCore& operator=(RendererCore&& rhs) = default;

		HWND GetWindowHandle() const;
		void SetWindowHandle(HWND windowHandle);

		void SetScreenResolution(uint32_t screenWidth, uint32_t screenHeight);
		uint32_t GetScreenWidth();
		uint32_t GetScreenHeight();

		void SetMultiSamplingCount(uint32_t multiSamplingCount);
		uint32_t GetMultiSamplingCount();
		uint32_t GetMultiSamplingQualityLevels();


		ID3D11Device2* GetDevice() const;
		ID3D11DeviceContext2* GetDeviceContext() const;
		IDXGISwapChain2* GetSwapChain() const;

		void InitializeD3DCore();

		bool IsWindowedMode() const;
		void SetWindowedMode(bool isWindowed);

	private:
		uint32_t ScreenWidth;
		uint32_t ScreenHeight;
		HWND WindowHandle;
		uint32_t MultiSamplingCount;
		uint32_t MultiSamplingQualityLevels;
		uint32_t BufferCount;

		uint32_t RefreshRate;
		bool IsWindowed;

		Microsoft::WRL::ComPtr<ID3D11Device2> Device;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext2> DeviceContext;
		Microsoft::WRL::ComPtr<IDXGISwapChain2> SwapChain;

		D3D_FEATURE_LEVEL FeatureLevel;
	};
}

