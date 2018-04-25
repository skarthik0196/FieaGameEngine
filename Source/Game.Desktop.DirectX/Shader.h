#pragma once
#include <d3d11_4.h>
#include <wrl.h>
#include <string>

namespace Rendering
{
	class Shader
	{
	public:

		enum class ShaderType
		{
			VertexShader,
			HullShader,
			DomainShader,
			GeometryShader,
			PixelShader,
			ComputeShader
		};

		Shader();
		Shader(const std::wstring& filePath, const ShaderType& type, ID3D11Device2* D3Ddevice);
		~Shader() = default;

		void InitializeShader(ID3D11Device2* D3Ddevice);

		ShaderType GetShaderType();
		void SetShaderType(const ShaderType& type);

		ID3D11VertexShader* GetVertexShader();
		ID3D11HullShader* GetHullShader();
		ID3D11DomainShader* GetDomainShader();
		ID3D11GeometryShader* GetGeometryShader();
		ID3D11PixelShader* GetPixelShader();
		ID3D11ComputeShader* GetComputeShader();



	private:
		void ReadShader(std::vector<char>& shaderData);

		ShaderType Type;

		Microsoft::WRL::ComPtr<ID3D11VertexShader> VertexShader;
		Microsoft::WRL::ComPtr<ID3D11HullShader> HullShader;
		Microsoft::WRL::ComPtr<ID3D11DomainShader> DomainShader;
		Microsoft::WRL::ComPtr<ID3D11GeometryShader> GeometryShader;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> PixelShader;
		Microsoft::WRL::ComPtr<ID3D11ComputeShader> ComputeShader;

		Microsoft::WRL::ComPtr<ID3D11InputLayout> InputLayout;

		std::wstring Path;
	};
}