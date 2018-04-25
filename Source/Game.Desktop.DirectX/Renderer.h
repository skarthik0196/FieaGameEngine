#pragma once
#include<Windows.h>
#include "RendererCore.h"
#include "CameraBase.h"

namespace Rendering
{
	class Model;

	class Renderer : public RendererCore
	{

	struct VertexCBufferPerObject
	{
		DirectX::XMFLOAT4X4 WorldViewProjectionMatrix;
		DirectX::XMFLOAT4X4 WorldMatrix;

		VertexCBufferPerObject();
		VertexCBufferPerObject(const DirectX::XMFLOAT4X4& WVP, const DirectX::XMFLOAT4X4& worldMatrix);
	};

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


		/// <summary>
		/// Temporary Stuff, needs to be moved out.
		/// </summary>
		
		std::shared_ptr<Model> TestModel;

	private:
		void SetCameraResolution();

		uint32_t MipLevels;

		std::vector<Microsoft::WRL::ComPtr<ID3D11RenderTargetView>> RenderTargetViews;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> DepthStencilView;

		std::shared_ptr<CameraBase> MainCamera;
	};
}