#include "pch.h"
#include "Shader.h"
#include <fstream>
#include <functional>
#include <cassert>

namespace Rendering
{
	Shader::Shader()
	{
	}

	Shader::Shader(const std::wstring& filePath, const ShaderType& type, ID3D11Device2* D3Ddevice) : Path(filePath), Type(type)
	{
		InitializeShader(D3Ddevice);
	}

	Shader::ShaderType Shader::GetShaderType()
	{
		return Type;
	}

	void Shader::SetShaderType(const ShaderType& type)
	{
		Type = type;
	}

	ID3D11VertexShader* Shader::GetVertexShader()
	{
		assert(Type == ShaderType::VertexShader);
		return VertexShader.Get();
	}

	ID3D11HullShader * Shader::GetHullShader()
	{
		assert(Type == ShaderType::HullShader);
		return HullShader.Get();
	}

	ID3D11DomainShader * Shader::GetDomainShader()
	{
		assert(Type == ShaderType::DomainShader);
		return DomainShader.Get();
	}

	ID3D11GeometryShader * Shader::GetGeometryShader()
	{
		assert(Type == ShaderType::GeometryShader);
		return GeometryShader.Get();
	}

	ID3D11PixelShader * Shader::GetPixelShader()
	{
		assert(Type == ShaderType::PixelShader);
		return PixelShader.Get();
	}

	ID3D11ComputeShader * Shader::GetComputeShader()
	{
		assert(Type == ShaderType::ComputeShader);
		return ComputeShader.Get();
	}

	void Shader::InitializeShader(ID3D11Device2* D3Ddevice)
	{
		std::vector<char> shaderByteCode;
		ReadShader(shaderByteCode);

		static std::vector<std::function<void(ID3D11Device2*, std::vector<char>&)>> shaderCreator =
		{
			[this](ID3D11Device2* device, std::vector<char>& byteCode) {ID3D11VertexShader* VS = this->GetVertexShader(); device->CreateVertexShader(byteCode.data(), byteCode.size(), nullptr, &VS);},
			[this](ID3D11Device2* device, std::vector<char>& byteCode) {ID3D11HullShader* HS = this->GetHullShader(); device->CreateHullShader(byteCode.data(), byteCode.size(), nullptr, &HS); },
			[this](ID3D11Device2* device, std::vector<char>& byteCode) {ID3D11DomainShader* DS = this->GetDomainShader(); device->CreateDomainShader(byteCode.data(), byteCode.size(), nullptr, &DS); },
			[this](ID3D11Device2* device, std::vector<char>& byteCode) {ID3D11GeometryShader* GS = this->GetGeometryShader(); device->CreateGeometryShader(byteCode.data(), byteCode.size(), nullptr, &GS); },
			[this](ID3D11Device2* device, std::vector<char>& byteCode) {ID3D11PixelShader* PS = this->GetPixelShader(); device->CreatePixelShader(byteCode.data(), byteCode.size(), nullptr, &PS); },
			[this](ID3D11Device2* device, std::vector<char>& byteCode) {ID3D11ComputeShader* CS = this->GetComputeShader(); device->CreateComputeShader(byteCode.data(), byteCode.size(), nullptr, &CS); }
		};

		shaderCreator[static_cast<uint32_t>(Type)](D3Ddevice, shaderByteCode);

		if (Type == ShaderType::VertexShader)
		{
			D3D11_INPUT_ELEMENT_DESC InputElementDescription[] =
			{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXTURECOORDINATE", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
			};

			D3Ddevice->CreateInputLayout(InputElementDescription, ARRAYSIZE(InputElementDescription), shaderByteCode.data(), shaderByteCode.size(), InputLayout.GetAddressOf());
		}
	}

	void Shader::ReadShader(std::vector<char>& shaderData)
	{
		std::ifstream file(Path.c_str(), std::ios::binary);
		if (file.bad())
		{
			MessageBox(nullptr, L"Error With File", L"Game.Desktop.DirectX", MB_ICONEXCLAMATION | MB_OK);
		}

		uint32_t size;
		file.seekg(0, std::ios::end);
		size = static_cast<uint32_t>(file.tellg());

		shaderData.reserve(size);

		file.seekg(0, std::ios::beg);
		file.read(shaderData.data(), size);
		file.close();
	}


}