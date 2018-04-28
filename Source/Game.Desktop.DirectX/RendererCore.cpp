#include "pch.h"
#include "RendererCore.h"

namespace Rendering
{
	RendererCore::RendererCore() : ScreenWidth(1024), ScreenHeight(768), MultiSamplingCount(4), BufferCount(2), RefreshRate(60), IsWindowed(true)
	{
	}

	RendererCore::RendererCore(HWND windowHandle, uint32_t screenWidth, uint32_t screenHeight) : WindowHandle(windowHandle), ScreenWidth(screenWidth), ScreenHeight(screenHeight), MultiSamplingCount(4), BufferCount(2), RefreshRate(60), IsWindowed(true)
	{
	}

	RendererCore::RendererCore(HWND windowHandle) : WindowHandle(windowHandle), ScreenWidth(1024), ScreenHeight(768), MultiSamplingCount(4), BufferCount(2), RefreshRate(60), IsWindowed(true)
	{
	}


	RendererCore::~RendererCore()
	{
	}

	HWND RendererCore::GetWindowHandle() const
	{
		return WindowHandle;
	}

	void RendererCore::SetWindowHandle(HWND windowHandle)
	{
		WindowHandle = windowHandle;
	}

	void RendererCore::SetScreenResolution(uint32_t screenWidth, uint32_t screenHeight)
	{
		ScreenWidth = screenWidth;
		ScreenHeight = screenHeight;
	}

	uint32_t RendererCore::GetScreenWidth()
	{
		return ScreenWidth;
	}

	uint32_t RendererCore::GetScreenHeight()
	{
		return ScreenHeight;
	}

	void RendererCore::SetMultiSamplingCount(uint32_t multiSamplingCount)
	{
		MultiSamplingCount = multiSamplingCount;
	}

	uint32_t RendererCore::GetMultiSamplingCount()
	{
		return MultiSamplingCount;
	}

	uint32_t RendererCore::GetMultiSamplingQualityLevels()
	{
		return MultiSamplingQualityLevels;
	}

	ID3D11Device2 * RendererCore::GetDevice() const
	{
		return Device.Get();
	}

	ID3D11DeviceContext2 * RendererCore::GetDeviceContext() const
	{
		return DeviceContext.Get();
	}

	IDXGISwapChain2 * RendererCore::GetSwapChain() const
	{
		return SwapChain.Get();
	}

	bool RendererCore::IsWindowedMode() const
	{
		return IsWindowed;
	}

	void RendererCore::SetWindowedMode(bool isWindowed)
	{
		IsWindowed = isWindowed;
	}

	void RendererCore::InitializeD3DCore()
	{
		D3D_FEATURE_LEVEL supportedFeatureLevels[] = { D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0 };

		HRESULT result;
		ID3D11Device *lDevice;
		ID3D11DeviceContext *lDeviceContext;

		DWORD createDeviceFlags = 0;
#ifdef _DEBUG
		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		result = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, supportedFeatureLevels, ARRAYSIZE(supportedFeatureLevels), D3D11_SDK_VERSION, &lDevice, &FeatureLevel, &lDeviceContext);
		result = lDevice->QueryInterface(__uuidof(ID3D11Device2), reinterpret_cast<void**>(Device.GetAddressOf()));
		result = lDeviceContext->QueryInterface(__uuidof(ID3D11DeviceContext2), reinterpret_cast<void**>(DeviceContext.GetAddressOf()));

		lDevice->Release();
		lDeviceContext->Release();

		result = Device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, MultiSamplingCount, &MultiSamplingQualityLevels);
		
		if (MultiSamplingQualityLevels <= 0)
		{
			throw std::runtime_error("Multisampling Error");
		}

		DXGI_SWAP_CHAIN_DESC1 swapChainDescription;
		ZeroMemory(&swapChainDescription, sizeof(swapChainDescription));

		swapChainDescription.Width = ScreenWidth;
		swapChainDescription.Height = ScreenHeight;
		swapChainDescription.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDescription.Stereo = false;
		swapChainDescription.SampleDesc.Count = MultiSamplingCount;
		swapChainDescription.SampleDesc.Quality = MultiSamplingQualityLevels - 1;
		swapChainDescription.BufferCount = 2;
		swapChainDescription.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDescription.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		swapChainDescription.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		IDXGIDevice1 *lDxgiDevice = nullptr;
		result = Device->QueryInterface(__uuidof(IDXGIDevice1), reinterpret_cast<void**>(&lDxgiDevice));

		IDXGIAdapter *lDxgiAdapter = nullptr;
		result = lDxgiDevice->GetAdapter(&lDxgiAdapter);

		IDXGIFactory2 *lDxgiFactory = nullptr;
		result = lDxgiAdapter->GetParent(__uuidof(IDXGIFactory2), reinterpret_cast<void**>(&lDxgiFactory));

		DXGI_SWAP_CHAIN_FULLSCREEN_DESC fullScreenDescription;
		ZeroMemory(&fullScreenDescription, sizeof(fullScreenDescription));

		fullScreenDescription.RefreshRate.Numerator = 60;
		fullScreenDescription.RefreshRate.Denominator = 1;
		fullScreenDescription.Windowed = true;

		IDXGISwapChain1* lSwapChain;

		result = lDxgiFactory->CreateSwapChainForHwnd(lDxgiDevice, WindowHandle, &swapChainDescription, &fullScreenDescription, nullptr, &lSwapChain);
		result = lSwapChain->QueryInterface(__uuidof(IDXGISwapChain2), reinterpret_cast<void**>(SwapChain.GetAddressOf()));

		lDxgiDevice->Release();
		lDxgiAdapter->Release();
		lDxgiFactory->Release();
	}
}
