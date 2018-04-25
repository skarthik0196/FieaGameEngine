#include "pch.h"
#include "Material.h"
#include <assimp\scene.h>

namespace Rendering
{
	Rendering::Material::Material()
	{
	}

	Rendering::Material::Material(aiMaterial* material, ID3D11Device2* D3DDevice)
	{
		aiString name;
		material->Get(AI_MATKEY_NAME, name);
		Name = name.C_Str();

		for (Texture::TextureType textureType = static_cast<Texture::TextureType>(0); textureType < Texture::TextureType::End; textureType = static_cast<Texture::TextureType>(static_cast<uint32_t>(textureType) + 1))
		{
			aiTextureType mappedTextureType = static_cast<aiTextureType>(TextureTypeMapping[textureType]);
			uint32_t textureCount = material->GetTextureCount(mappedTextureType);
			if (textureCount > 0)
			{
				for (uint32_t textureIndex = 0; textureIndex < textureCount; ++textureIndex)
				{
					aiString path;
					if (material->GetTexture(mappedTextureType, textureIndex, &path) == AI_SUCCESS)
					{
						std::string sPath = path.C_Str();
						std::wstring wPath = wPath.assign(sPath.begin(), sPath.end());
						CreateTexture(D3DDevice, wPath, Texture::TextureFileType::WIC, textureType);
					}
				}
			}
		}
	}

	const std::string & Rendering::Material::GetName() const
	{
		return Name;
	}

	std::map<Texture::TextureType, std::vector<std::shared_ptr<Texture>>>& Material::GetTextures()
	{
		return Textures;
	}

	void Material::CreateTexture(ID3D11Device2* D3Ddevice, const std::wstring & filePath, Texture::TextureFileType fileType, Texture::TextureType textureType)
	{
		auto newTexture = std::make_shared<Texture>(filePath, fileType, textureType);
		auto textureTypeVector = Textures[textureType];
		textureTypeVector.push_back(newTexture);
		newTexture->InitializeTexture(D3Ddevice);
	}

	std::vector<std::shared_ptr<Texture>>& Material::GetTexturesofType(const Texture::TextureType& textureType)
	{
		return Textures[textureType];
	}

	std::map<Texture::TextureType, uint32_t> Material::TextureTypeMapping =
	{
		{ Texture::TextureType::Diffuse, aiTextureType_DIFFUSE },
		{ Texture::TextureType::Specular, aiTextureType_SPECULAR },
		{ Texture::TextureType::Ambient, aiTextureType_AMBIENT },
		{ Texture::TextureType::Emissive, aiTextureType_EMISSIVE },
		{ Texture::TextureType::HeightMap, aiTextureType_HEIGHT },
		{ Texture::TextureType::NormalMap, aiTextureType_NORMALS },
		{ Texture::TextureType::SpecularPowerMap, aiTextureType_SHININESS },
		{ Texture::TextureType::DisplacementMap, aiTextureType_DISPLACEMENT },
		{ Texture::TextureType::LightMap, aiTextureType_LIGHTMAP }
	};
}