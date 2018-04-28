#pragma once
#include "pch.h"
#include "Renderer.h"
#include<DirectXPackedVector.h>
#include "Model.h"
#include "Mesh.h"
#include "Material.h"
#include "Texture.h"
#include "Shader.h"
#include <Shlwapi.h>

namespace Rendering
{
	static DirectX::XMVECTORF32 BGColor = { 0.0f,0.2f,0.4f,1.0f };

	Renderer::Renderer() : RendererCore(), MainCamera(std::make_shared<CameraBase>(0.01f, 10000.0f)), WorldMatrix(DirectX::XMFLOAT4X4(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f))
	{
		SetCameraResolution();
	}

	Renderer::Renderer(HWND windowHandle, int screenWidth, int screenHeight) : RendererCore(windowHandle, screenWidth, screenHeight), MainCamera(std::make_shared<CameraBase>()), WorldMatrix(DirectX::XMFLOAT4X4(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f))
	{
		SetCameraResolution();
	}

	Renderer::Renderer(HWND windowHandle) : RendererCore(windowHandle), MainCamera(std::make_shared<CameraBase>()), WorldMatrix(DirectX::XMFLOAT4X4(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f))
	{
		SetCameraResolution();
	}

	Renderer::~Renderer()
	{
	}

	void Renderer::SetCameraResolution()
	{
		MainCamera->SetScreenHeight(static_cast<float>(GetScreenHeight()));
		MainCamera->SetScreenWidth(static_cast<float>(GetScreenWidth()));
		MainCamera->InitializePerspectiveProjectionMatrix();
	}

	std::shared_ptr<CameraBase> Renderer::GetMainCamera()
	{
		return MainCamera;
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

		GetDeviceContext()->OMSetRenderTargets(static_cast<uint32_t>(RenderTargetViews.size()), RenderTargetViews[0].GetAddressOf(), DepthStencilView.Get());
		CreateViewPort();

		WCHAR PathBuffer[MAX_PATH];
		GetModuleFileName(nullptr, PathBuffer, MAX_PATH);
		PathRemoveFileSpec(PathBuffer);
		std::wstring Path(PathBuffer);
		SetCurrentDirectory(Path.c_str());

		QuadBatch = std::make_shared<QuadBatcher>(GetDevice());

		// Put the following Code in appropriate place later

		MainCamera->SetPosition(DirectX::XMFLOAT3(0.0f, 2.0f, -30.0f));
		//MainCamera->Rotate(DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f), 15.0f);

		TestModel = std::make_shared<Model>(GetDevice(), R"(Content\Models\Sphere.obj)", true);
		TestModel->GetMeshes()[0]->GetMaterial()->CreateTexture(GetDevice(), L"Content\\Textures\\EarthComposite.dds", Texture::TextureFileType::DDS, Texture::TextureType::Diffuse);
		VS = std::make_shared<Shader>(L"BasicVertexShader.cso", Shader::ShaderType::VertexShader, GetDevice());
		PS = std::make_shared<Shader>(L"BasicPixelShader.cso", Shader::ShaderType::PixelShader, GetDevice());

		QVS = std::make_shared<Shader>(L"QuadVertexShader.cso", Shader::ShaderType::VertexShader, GetDevice());
		QPS = std::make_shared<Shader>(L"QuadPixelShader.cso", Shader::ShaderType::PixelShader, GetDevice());

		D3D11_SAMPLER_DESC SamplerDescription;
		ZeroMemory(&SamplerDescription, sizeof(SamplerDescription));
		SamplerDescription.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;	//Trilinear filtering
		SamplerDescription.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		SamplerDescription.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		SamplerDescription.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

		GetDevice()->CreateSamplerState(&SamplerDescription, ColorSampler.GetAddressOf());

		D3D11_BUFFER_DESC ConstantBufferDescription;
		ZeroMemory(&ConstantBufferDescription, sizeof(ConstantBufferDescription));

		ConstantBufferDescription.ByteWidth = sizeof(VertexCBufferPerObject);
		ConstantBufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

		GetDevice()->CreateBuffer(&ConstantBufferDescription, nullptr, &VSCBufferObject);

		TestQuad = std::make_shared<Quad>(GetDevice(), QuadBatch);
		TestQuad->SetPosition(DirectX::XMFLOAT3(1.0f, -1.0f, 0.0f));
		TestQuad->SetRotation(DirectX::XMFLOAT3(0.0f, 45.0f, 0.0f));
		TestQuad->SetScale(DirectX::XMFLOAT3(10.0f,10.0f, 10.0f));
		TestQuad->SetColor(DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f));

		TestQuad2 = std::make_shared<Quad>(GetDevice(), QuadBatch);
		TestQuad2->SetPosition(DirectX::XMFLOAT3(-1.0f, -1.0f, 0.0f));
		TestQuad2->SetRotation(DirectX::XMFLOAT3(0.0f, -45.0f, 0.0f));
		TestQuad2->SetScale(DirectX::XMFLOAT3(10.0f, 10.0f, 10.0f));
		TestQuad2->SetColor(DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));

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

	void Renderer::Update(std::chrono::milliseconds& deltaTime)
	{
		deltaTime;
		static float angle = 0.0f;
		DirectX::XMStoreFloat4x4(&WorldMatrix, DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(angle)));
		//MainCamera->Rotate(DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f), angle);
		angle = angle + 0.01f;//(1.0f * deltaTime.count());
	}

	void Renderer::RenderFrame()
	{
		std::shared_ptr<Mesh> mesh = TestModel->GetMeshes()[0];

		ID3D11DeviceContext2* deviceContext = GetDeviceContext();

		deviceContext->ClearRenderTargetView(RenderTargetViews[0].Get(), reinterpret_cast<float*>(&BGColor));
		deviceContext->ClearDepthStencilView(DepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		deviceContext->VSSetShader(VS->GetVertexShader(), 0, 0);
		deviceContext->PSSetShader(PS->GetPixelShader(), 0, 0);

		uint32_t stride = sizeof(Vertex);
		uint32_t offset = 0;

		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		//auto* vertexBuffer = TestModel->GetMeshes()[0]->GetVertexBuffer();
		deviceContext->IASetVertexBuffers(0, 1, mesh->GetAddressOfVertexBuffer(), &stride, &offset);
		deviceContext->IASetIndexBuffer(mesh->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);
		deviceContext->IASetInputLayout(VS->GetInputLayout());

		DirectX::XMMATRIX viewProjectionMatrix = MainCamera->GetViewProjectionMatrix();
		DirectX::XMMATRIX WVP = (DirectX::XMLoadFloat4x4(&WorldMatrix) * viewProjectionMatrix);
		WVP = DirectX::XMMatrixTranspose(WVP);
		DirectX::XMStoreFloat4x4(&(VSCBuffer.WorldViewProjectionMatrix), WVP);
		DirectX::XMStoreFloat4x4(&(VSCBuffer.WorldMatrix), DirectX::XMMatrixTranspose(DirectX::XMLoadFloat4x4(&WorldMatrix)));

		deviceContext->UpdateSubresource(VSCBufferObject, 0, nullptr, &VSCBuffer, 0, 0);

		deviceContext->VSSetConstantBuffers(0, 1, &VSCBufferObject);

		deviceContext->PSSetShaderResources(0, 1, mesh->GetMaterial()->GetTexturesofType(Texture::TextureType::Diffuse)[0]->GetAdddressOfShaderResource());
		deviceContext->PSSetSamplers(0, 1, ColorSampler.GetAddressOf());

		deviceContext->DrawIndexed(mesh->GetIndexCount(), 0, 0);

		////////////////////////////////////////////////////////////

		//Single Quad Draw

		/*DirectX::XMMATRIX testQuadWorldMatrix = TestQuad->GetWorldMatrix();
		WVP = testQuadWorldMatrix * viewProjectionMatrix;
		WVP = DirectX::XMMatrixTranspose(WVP);
		deviceContext->VSSetShader(QVS->GetVertexShader(), 0, 0);
		deviceContext->PSSetShader(QPS->GetPixelShader(), 0, 0);

		DirectX::XMStoreFloat4x4(&(VSCBuffer.WorldViewProjectionMatrix), WVP);
		DirectX::XMStoreFloat4x4(&(VSCBuffer.WorldMatrix), DirectX::XMMatrixTranspose(testQuadWorldMatrix));

		deviceContext->UpdateSubresource(VSCBufferObject, 0, nullptr, &VSCBuffer, 0, 0);

		deviceContext->IASetVertexBuffers(0, 1, TestQuad->GetAddressofVertexBuffer(), &stride, &offset);

		deviceContext->Draw(TestQuad->GetVertexCount(), 0);*/


		//Batch Draw
		QuadBatch->BatchDraw(deviceContext, viewProjectionMatrix);

		GetSwapChain()->Present(0,0);
	}

	Renderer::VertexCBufferPerObject::VertexCBufferPerObject()
	{
	}

	Renderer::VertexCBufferPerObject::VertexCBufferPerObject(const DirectX::XMFLOAT4X4 & WVP, const DirectX::XMFLOAT4X4 & worldMatrix) : WorldViewProjectionMatrix(WVP), WorldMatrix(worldMatrix)
	{

	}
}