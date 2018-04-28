#include "pch.h"
#include "Quad.h"
#include "QuadBatcher.h"

namespace Rendering
{
	Quad::Quad(ID3D11Device2* D3DDevice, std::shared_ptr<QuadBatcher> quadBatcher) : Vertices
	(
		{
			Vertex(DirectX::XMFLOAT4(-0.5f,0.0f,0.0f,1.0f), DirectX::XMFLOAT2(0.0f,1.0f), DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f)),
			Vertex(DirectX::XMFLOAT4(-0.5f,1.0f,0.0f,1.0f), DirectX::XMFLOAT2(1.0f,1.0f), DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f)),
			Vertex(DirectX::XMFLOAT4(0.5f,1.0f,0.0f,1.0f), DirectX::XMFLOAT2(1.0f,0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f)),
																												  	    
			Vertex(DirectX::XMFLOAT4(-0.5f,0.0f,0.0f,1.0f), DirectX::XMFLOAT2(0.0f, 1.0f), DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f)),
			Vertex(DirectX::XMFLOAT4(0.5f,1.0f,0.0f,1.0f), DirectX::XMFLOAT2(1.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f)),
			Vertex(DirectX::XMFLOAT4(0.5f,0.0f,0.0f,1.0f), DirectX::XMFLOAT2(1.0f, 1.0f), DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f)),
		}
	), Position(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f)), Rotation(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f)), Scale(DirectX::XMFLOAT3(10.0f, 10.0f, 10.0f)), Color(DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f)), Fill(FillMode::SolidColor)
	{
		InitializeVertexBuffer(D3DDevice);

		if (quadBatcher != nullptr)
		{
			Index = quadBatcher->AddQuad(*this, D3DDevice);
		}
	}

	Quad::~Quad()
	{
	}

	const std::vector<Vertex>& Quad::GetVertices()
	{
		return Vertices;
	}

	uint32_t Quad::GetVertexCount()
	{
		return static_cast<uint32_t>(Vertices.size());
	}

	ID3D11Buffer* Quad::GetVertexBuffer()
	{
		return VertexBuffer.Get();
	}

	ID3D11Buffer** Quad::GetAddressofVertexBuffer()
	{
		return VertexBuffer.GetAddressOf();
	}

	void Quad::SetPosition(const DirectX::XMFLOAT3 & position)
	{
		Position = position;
	}

	void Quad::SetRotation(const DirectX::XMFLOAT3 & rotation)
	{
		Rotation = rotation;
	}

	void Quad::SetScale(const DirectX::XMFLOAT3 & scale)
	{
		Scale = scale;
	}

	void Quad::SetColor(const DirectX::XMFLOAT4& color)
	{
		Color = color;
	}

	void Quad::SetFillMode(const FillMode & fillMode)
	{
		Fill = fillMode;
	}

	DirectX::XMMATRIX Quad::GetWorldMatrix()
	{
		DirectX::XMMATRIX WorldMatrix;
		WorldMatrix = DirectX::XMMatrixMultiply(DirectX::XMMatrixTranslation(Position.x, Position.y, Position.z), DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(Rotation.x)));
		WorldMatrix = WorldMatrix * DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(Rotation.y)) * DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(Rotation.z)) * DirectX::XMMatrixScaling(Scale.x, Scale.y, Scale.z);

		return WorldMatrix;
	}

	DirectX::XMFLOAT4X4 Quad::GetWorldMatrixAsFloat4x4()
	{
		return DirectX::XMFLOAT4X4();
	}

	const DirectX::XMFLOAT3& Quad::GetPosition()
	{
		return Position;
	}

	const DirectX::XMFLOAT3& Quad::GetRotation()
	{
		return Rotation;
	}

	const DirectX::XMFLOAT3& Quad::GetScale()
	{
		return Scale;
	}

	const DirectX::XMFLOAT4& Quad::GetColor()
	{
		return Color;
	}

	const Quad::FillMode& Quad::GetFillMode()
	{
		return Fill;
	}

	void Quad::InitializeVertexBuffer(ID3D11Device2* D3DDevice)
	{
		D3D11_BUFFER_DESC vertexBufferDescription;
		ZeroMemory(&vertexBufferDescription, sizeof(vertexBufferDescription));

		vertexBufferDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDescription.ByteWidth = sizeof(Vertex) * static_cast<uint32_t>(Vertices.size());
		vertexBufferDescription.Usage = D3D11_USAGE_IMMUTABLE;

		D3D11_SUBRESOURCE_DATA vertexSubresourceData = { 0 };
		vertexSubresourceData.pSysMem = &Vertices[0];
		D3DDevice->CreateBuffer(&vertexBufferDescription, &vertexSubresourceData, &VertexBuffer);
	}

	uint32_t Quad::GetIndex()
	{
		return Index;
	}

	void Quad::SetIndex(uint32_t index)
	{
		Index = index;
	}


}