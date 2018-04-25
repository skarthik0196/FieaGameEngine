#include "pch.h"
#include "Texture.h"
#include <DDSTextureLoader.h>
#include <WICTextureLoader.h>
#include <functional>


namespace Rendering
{
	Texture::Texture(const std::wstring& name, TextureFileType fileType, TextureType textureType) :Name(name), FileType(fileType), Type(textureType)
	{

	}

	Texture::TextureType Texture::GetTextureType()
	{
		return Type;
	}

	Texture::TextureFileType Texture::GetTextureFileType()
	{
		return FileType;
	}

	void Texture::SetTextureType(const TextureType& textureType)
	{
		Type = textureType;
	}

	void Texture::SetTextureFileType(const TextureFileType& textureFileType)
	{
		FileType = textureFileType;
	}

	ID3D11ShaderResourceView* Texture::GetShaderResouce()
	{
		return ShaderResource.Get();
	}

	const std::wstring& Texture::GetFilePath()
	{
		return Name;
	}

	void Texture::SetFilePath(const std::wstring& FilePath)
	{
		Name = FilePath;
	}

	void Texture::InitializeTexture(ID3D11Device2* device)
	{
		if (FileType == TextureFileType::DDS)
		{
			DirectX::CreateDDSTextureFromFile(device, Name.c_str(), nullptr, &ShaderResource);
		}
		else
		{
			DirectX::CreateWICTextureFromFile(device, Name.c_str(), nullptr, &ShaderResource);
		}
	}
}