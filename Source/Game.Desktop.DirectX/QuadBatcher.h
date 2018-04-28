#pragma once
#include"Quad.h"
#include<d3d11_4.h>
#include<wrl.h>

namespace Rendering
{
#define QuadLimitPerBatch 512

	class Shader;
	class QuadBatcher
	{
	public:

		struct QuadVertex
		{
			DirectX::XMFLOAT4 Position;
			DirectX::XMFLOAT2 TextureCoordinates;
			DirectX::XMFLOAT3 Normals;
			uint32_t Index;

			QuadVertex(const DirectX::XMFLOAT4& position, const DirectX::XMFLOAT2& textureCoordinates, const DirectX::XMFLOAT3& normals, uint32_t index);
		};

		struct BufferPerQuad
		{
			DirectX::XMFLOAT4X4 WorldMatrix;
			DirectX::XMFLOAT4 Color;
		};

		struct CBufferForAllQuads
		{
			BufferPerQuad QuadCBuffers[QuadLimitPerBatch];
			DirectX::XMFLOAT4X4 ViewProjectionMatrix;
		};

		explicit QuadBatcher(ID3D11Device2* D3DDevice);
		virtual ~QuadBatcher();

		void InitializeBatchedVertexBuffer(ID3D11Device2* D3DDevice);

		void InitializeWVPArray(DirectX::XMMATRIX viewProjectionMatrix);

		uint32_t AddQuad(Quad& newQuad, ID3D11Device2* D3DDevice);
		void RemoveQuad(Quad& quad, ID3D11Device2* D3DDevice);

		void BatchDraw(ID3D11DeviceContext* deviceContext, DirectX::XMMATRIX viewProjectionMatrix);

		void ReintializeVertexData(ID3D11Device2* D3DDevice);

	private:
		void InitializeConstantBuffer(ID3D11Device2* D3DDevice);

		Microsoft::WRL::ComPtr<ID3D11Buffer> BatchedBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> QuadCBuffer;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> TBufferData;

		std::vector<Quad*> QuadList;
		std::vector<QuadVertex> QuadVertexList;

		CBufferForAllQuads VSCBufferForAllQuads;

		std::shared_ptr<Shader> BatchedVertexShader;
		std::shared_ptr<Shader> BatchedPixelShader;

		static D3D11_INPUT_ELEMENT_DESC QuadInputDescription[4];
	};
}