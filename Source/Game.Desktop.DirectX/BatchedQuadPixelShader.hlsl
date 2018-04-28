struct VS_OUTPUT
{
	float4 Position : SV_POSITION;
	float2 TextureCoordinate : TEXTURECOORDINATE;
	float4 Color : COLOR;
};

SamplerState ColorSampler;
Texture2D QuadTexture;

float4 main(VS_OUTPUT IN) : SV_TARGET
{
	float4 OutputColor = (float4)0;
	OutputColor = IN.Color;

	return OutputColor;
}