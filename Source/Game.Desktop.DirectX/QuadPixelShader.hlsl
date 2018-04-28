struct VS_OUTPUT
{
	float4 Position : SV_POSITION;
	float2 TextureCoordinate : TEXTURECOORDINATE;
};

SamplerState ColorSampler;
Texture2D QuadTexture;

float4 main() : SV_TARGET
{
	float4 OutputColor = (float4)0;
	return float4(1.0f, 1.0f, 1.0f, 1.0f);
}