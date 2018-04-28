#include "pch.h"
#include "QuadBatcher.h"
#include "Shader.h"

namespace Rendering
{
	D3D11_INPUT_ELEMENT_DESC QuadBatcher::QuadInputDescription[4] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXTURECOORDINATE", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "INDEX",0, DXGI_FORMAT_R32_UINT,0,D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	QuadBatcher::QuadBatcher(ID3D11Device2* D3DDevice) : BatchedVertexShader(std::make_shared<Shader>(L"BatchedQuadVertexShader.cso", Shader::ShaderType::VertexShader, D3DDevice, QuadInputDescription, ARRAYSIZE(QuadInputDescription))), BatchedPixelShader(std::make_shared<Shader>(L"BatchedQuadPixelShader.cso", Shader::ShaderType::PixelShader, D3DDevice))
	{
		InitializeConstantBuffer(D3DDevice);
	}


	QuadBatcher::~QuadBatcher()
	{
	}

	void QuadBatcher::InitializeBatchedVertexBuffer(ID3D11Device2* D3DDevice)
	{
		BatchedBuffer.Reset();

		D3D11_BUFFER_DESC vertexBufferDescription;
		ZeroMemory(&vertexBufferDescription, sizeof(vertexBufferDescription));

		vertexBufferDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDescription.ByteWidth = sizeof(QuadVertex) * static_cast<uint32_t>(QuadVertexList.size());
		vertexBufferDescription.Usage = D3D11_USAGE_IMMUTABLE;

		D3D11_SUBRESOURCE_DATA vertexSubresourceData = { 0 };
		vertexSubresourceData.pSysMem = &QuadVertexList[0];

		HRESULT res;
		res = D3DDevice->CreateBuffer(&vertexBufferDescription, &vertexSubresourceData, &BatchedBuffer);
	}

	void QuadBatcher::InitializeWVPArray(DirectX::XMMATRIX viewProjectionMatrix)
	{
		DirectX::XMStoreFloat4x4(&VSCBufferForAllQuads.ViewProjectionMatrix, DirectX::XMMatrixTranspose(viewProjectionMatrix));

		if (QuadVertexList.size() < QuadLimitPerBatch)
		{
			for (auto& vertex : QuadList)
			{
				BufferPerQuad BPQ;
				DirectX::XMMATRIX worldMatrix = vertex->GetWorldMatrix();
				DirectX::XMStoreFloat4x4(&BPQ.WorldMatrix, DirectX::XMMatrixTranspose(worldMatrix));
				BPQ.Color = vertex->GetColor();

				VSCBufferForAllQuads.QuadCBuffers[vertex->GetIndex()] = BPQ;
			}
		}
	}

	uint32_t QuadBatcher::AddQuad(Quad& newQuad, ID3D11Device2* D3DDevice)
	{
		QuadList.push_back(&newQuad);
		
		const std::vector<Vertex>& vertices = newQuad.GetVertices();
		
		uint32_t index = static_cast<uint32_t>(QuadList.size()) - 1;

		for (auto& vertex : vertices)
		{
			QuadVertex indexedVertex(vertex.Position, vertex.TextureCoordinates, vertex.Normals, index);
			QuadVertexList.push_back(indexedVertex);
		}

		InitializeBatchedVertexBuffer(D3DDevice);

		return index;
	}

	void QuadBatcher::RemoveQuad(Quad& quad, ID3D11Device2* D3DDevice)
	{
		auto It = std::find(QuadList.begin(), QuadList.end(), &quad);
		if (It != QuadList.end())
		{
			QuadList.erase(It);
			ReintializeVertexData(D3DDevice);
		}
	}

	//Implement
	void QuadBatcher::BatchDraw(ID3D11DeviceContext* deviceContext, DirectX::XMMATRIX viewProjectionMatrix)
	{
		InitializeWVPArray(viewProjectionMatrix);

		deviceContext->VSSetShader(BatchedVertexShader->GetVertexShader(), 0, 0);
		deviceContext->PSSetShader(BatchedPixelShader->GetPixelShader(), 0, 0);

		uint32_t offset = 0;
		uint32_t stride = sizeof(QuadVertex);

		deviceContext->IASetVertexBuffers(0, 1, BatchedBuffer.GetAddressOf(), &stride, &offset);
		deviceContext->IASetInputLayout(BatchedVertexShader->GetQuadInputLayout());

		deviceContext->UpdateSubresource(QuadCBuffer.Get(), 0, nullptr, &VSCBufferForAllQuads, 0, 0);
		deviceContext->VSSetConstantBuffers(0, 1, QuadCBuffer.GetAddressOf());
		//deviceContext->VSSetShaderResources(0, 1, TBufferData.GetAddressOf());

		deviceContext->Draw(QuadVertexList.size(), 0);
		
	}

	void QuadBatcher::ReintializeVertexData(ID3D11Device2* D3DDevice)
	{
		QuadVertexList.clear();

		for (uint32_t i = 0; i < static_cast<uint32_t>(QuadList.size()); ++i)
		{
			const std::vector<Vertex>& vertices = QuadList[i]->GetVertices();

			for (auto& vertex : vertices)
			{
				QuadVertex indexedVertex(vertex.Position, vertex.TextureCoordinates, vertex.Normals, i);
				QuadVertexList.push_back(indexedVertex);
			}

			QuadList[i]->SetIndex(i);
		}

		InitializeBatchedVertexBuffer(D3DDevice);
	}

	void QuadBatcher::InitializeConstantBuffer(ID3D11Device2 * D3DDevice)
	{
		D3D11_BUFFER_DESC constantBufferDescription;
		ZeroMemory(&constantBufferDescription, sizeof(D3D11_BUFFER_DESC));

		constantBufferDescription.ByteWidth = sizeof(CBufferForAllQuads);
		constantBufferDescription.BindFlags = D3D11_BIND_SHADER_RESOURCE;

		D3DDevice->CreateBuffer(&constantBufferDescription, nullptr, QuadCBuffer.GetAddressOf());

		//D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceDescription;
		//ZeroMemory(&shaderResourceDescription, sizeof(shaderResourceDescription));

		//shaderResourceDescription.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		//shaderResourceDescription.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;


		D3DDevice->CreateShaderResourceView(QuadCBuffer.Get(), nullptr, TBufferData.GetAddressOf());
	}

	QuadBatcher::QuadVertex::QuadVertex(const DirectX::XMFLOAT4& position, const DirectX::XMFLOAT2& textureCoordinates, const DirectX::XMFLOAT3& normals, uint32_t index) : Position(position), TextureCoordinates(textureCoordinates), Normals(normals), Index(index)
	{

	}
}