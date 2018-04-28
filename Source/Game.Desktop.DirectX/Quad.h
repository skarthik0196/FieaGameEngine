#pragma once
#include "Mesh.h"
#include <d3d11_4.h>
#include <wrl.h>
#include <memory>

namespace Rendering
{

	class QuadBatcher;
	class Quad : public std::enable_shared_from_this<Quad>
	{
	public:

		

		enum class FillMode
		{
			SolidColor,
			TexturedSprite
		};

		explicit Quad(ID3D11Device2* D3DDevice, std::shared_ptr<QuadBatcher> quadBatcher = nullptr);
		~Quad();

		const std::vector<Vertex>& GetVertices();
		uint32_t GetVertexCount();

		ID3D11Buffer* GetVertexBuffer();
		ID3D11Buffer** GetAddressofVertexBuffer();

		void SetPosition(const DirectX::XMFLOAT3& position);
		void SetRotation(const DirectX::XMFLOAT3& rotation);
		void SetScale(const DirectX::XMFLOAT3& scale);
		void SetColor(const DirectX::XMFLOAT4& color);
		void SetFillMode(const FillMode& fillMode);

		DirectX::XMMATRIX GetWorldMatrix();
		DirectX::XMFLOAT4X4 GetWorldMatrixAsFloat4x4();

		const DirectX::XMFLOAT3& GetPosition();
		const DirectX::XMFLOAT3& GetRotation();
		const DirectX::XMFLOAT3& GetScale();
		const DirectX::XMFLOAT4& GetColor();
		const FillMode& GetFillMode();

		void InitializeVertexBuffer(ID3D11Device2* D3DDevice);

		uint32_t GetIndex();
		void SetIndex(uint32_t index);

	private:
		FillMode Fill;
		int Index;
		
		DirectX::XMFLOAT3 Position;
		DirectX::XMFLOAT3 Rotation;
		DirectX::XMFLOAT3 Scale;
		DirectX::XMFLOAT4 Color;

		std::vector<Vertex> Vertices;
		Microsoft::WRL::ComPtr<ID3D11Buffer> VertexBuffer;
	};

}