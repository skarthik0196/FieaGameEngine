Texture2D ColorTexture;
SamplerState ColorSampler;

struct VS_OUTPUT
{
	float4 Position : SV_POSITION;
	float2 TextureCoordinate : TEXTURECOORDINATE;
};

float4 main(VS_OUTPUT IN) : SV_TARGET
{
	float4 OutputColor = (float4)0;

	OutputColor = ColorTexture.Sample(ColorSampler, IN.TextureCoordinate);

	return OutputColor;
}