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

	ID3D11InputLayout* Shader::GetInputLayout()
	{
		return InputLayout.Get();
	}

	void Shader::InitializeShader(ID3D11Device2* D3Ddevice)
	{
		std::vector<char> shaderByteCode;
		ReadShader(shaderByteCode);

		/*static std::vector<std::function<void(ID3D11Device2*, std::vector<char>&, Shader& shader)>> shaderCreator =
		{
			[](ID3D11Device2* device, std::vector<char>& byteCode, Shader& shader) {ID3D11VertexShader* VS = shader.GetVertexShader(); device->CreateVertexShader(byteCode.data(), byteCode.size(), nullptr, &VS); },
			[](ID3D11Device2* device, std::vector<char>& byteCode, Shader& shader) {ID3D11HullShader* HS = shader.GetHullShader(); device->CreateHullShader(byteCode.data(), byteCode.size(), nullptr, &HS); },
			[](ID3D11Device2* device, std::vector<char>& byteCode, Shader& shader) {ID3D11DomainShader* DS = shader.GetDomainShader(); device->CreateDomainShader(byteCode.data(), byteCode.size(), nullptr, &DS); },
			[](ID3D11Device2* device, std::vector<char>& byteCode, Shader& shader) {ID3D11GeometryShader* GS = shader.GetGeometryShader(); device->CreateGeometryShader(byteCode.data(), byteCode.size(), nullptr, &GS); },
			[](ID3D11Device2* device, std::vector<char>& byteCode, Shader& shader) {ID3D11PixelShader* PS = shader.GetPixelShader(); device->CreatePixelShader(byteCode.data(), byteCode.size(), nullptr, &PS); },
			[](ID3D11Device2* device, std::vector<char>& byteCode, Shader& shader) {ID3D11ComputeShader* CS = shader.GetComputeShader(); device->CreateComputeShader(byteCode.data(), byteCode.size(), nullptr, &CS); }
		};*/

		static void (Shader::*shaderCreator[6])(ID3D11Device2*, std::vector<char>&) = { &Shader::CreateVertexShader, &Shader::CreateHullShader, &Shader::CreateDomainShader, 
																						&Shader::CreateGeometryShader, &Shader::CreatePixelShader, &Shader::CreateComputeShader };

		(this->*shaderCreator[static_cast<uint32_t>(Type)])(D3Ddevice, shaderByteCode);

		/*HRESULT res = D3Ddevice->CreateVertexShader(shaderByteCode.data(), shaderByteCode.size(), nullptr, VertexShader.GetAddressOf());
		res;*/

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

		shaderData.resize(size);

		file.seekg(0, std::ios::beg);
		file.read(shaderData.data(), size);
		file.close();
	}

	void Shader::CreateVertexShader(ID3D11Device2 * D3Ddevice, std::vector<char>& byteCode)
	{
		D3Ddevice->CreateVertexShader(byteCode.data(), byteCode.size(), nullptr, VertexShader.GetAddressOf());
	}

	void Shader::CreateHullShader(ID3D11Device2 * D3Ddevice, std::vector<char>& byteCode)
	{
		D3Ddevice->CreateHullShader(byteCode.data(), byteCode.size(), nullptr, HullShader.GetAddressOf());
	}

	void Shader::CreateDomainShader(ID3D11Device2 * D3Ddevice, std::vector<char>& byteCode)
	{
		D3Ddevice->CreateDomainShader(byteCode.data(), byteCode.size(), nullptr, DomainShader.GetAddressOf());
	}

	void Shader::CreateGeometryShader(ID3D11Device2 * D3Ddevice, std::vector<char>& byteCode)
	{
		D3Ddevice->CreateGeometryShader(byteCode.data(), byteCode.size(), nullptr, GeometryShader.GetAddressOf());
	}

	void Shader::CreatePixelShader(ID3D11Device2 * D3Ddevice, std::vector<char>& byteCode)
	{
		D3Ddevice->CreatePixelShader(byteCode.data(), byteCode.size(), nullptr, PixelShader.GetAddressOf());
	}

	void Shader::CreateComputeShader(ID3D11Device2 * D3Ddevice, std::vector<char>& byteCode)
	{
		D3Ddevice->CreateComputeShader(byteCode.data(), byteCode.size(), nullptr, ComputeShader.GetAddressOf());
	}
}