#pragma once
#include <d3d11_4.h>
#include <wrl.h>

namespace Rendering
{
	class Texture
	{
	public:

		enum class TextureType
		{
			Diffuse = 0,
			Specular,
			Ambient,
			Emissive,
			HeightMap,
			NormalMap,
			SpecularPowerMap,
			DisplacementMap,
			LightMap,
			End
		};

		enum class TextureFileType
		{
			DDS,
			WIC
		};

		Texture(const std::wstring& name, TextureFileType fileType, TextureType textureType = TextureType::Diffuse);
		virtual ~Texture() = default;

		TextureType GetTextureType();
		TextureFileType GetTextureFileType();

		void SetTextureType(const TextureType& textureType);
		void SetTextureFileType(const TextureFileType& textureFileType);

		ID3D11ShaderResourceView* GetShaderResouce();

		const std::wstring& GetFilePath();
		void SetFilePath(const std::wstring& FilePath);

		void InitializeTexture(ID3D11Device2* device);

	private:
		std::wstring Name;
		TextureFileType FileType;
		TextureType Type;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> ShaderResource;
	};
}