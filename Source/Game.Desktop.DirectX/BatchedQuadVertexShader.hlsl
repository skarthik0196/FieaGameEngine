struct CBufferPerQuad
{
	float4x4 WorldMatrix : WORLDMATRIX;
	float4 Color : COLOR;
};

cbuffer CBufferForAllQuads : register (b0)
{
	CBufferPerQuad QuadCBufferArray[512];
	float4x4 ViewProjectionMatrix;
};

struct VS_INPUT
{
	float4 Position :POSITION;
	float2 TextureCoordinate : TEXTURECOORDINATE;
	float3 Normal : NORMAL;
	uint Index : INDEX;
};

struct VS_OUTPUT
{
	float4 Position : SV_POSITION;
	float2 TextureCoordinate : TEXTURECOORDINATE;
	float4 Color : COLOR;
};

VS_OUTPUT main(VS_INPUT IN)
{
	VS_OUTPUT OUT = (VS_OUTPUT)0;
	CBufferPerQuad QuadData = QuadCBufferArray[IN.Index];

	float4x4 WorldViewProjectionMatrix = mul(QuadData.WorldMatrix, ViewProjectionMatrix);
	OUT.Position = mul(IN.Position, WorldViewProjectionMatrix);
	OUT.TextureCoordinate = IN.TextureCoordinate;
	OUT.Color = QuadData.Color;
	return OUT;
}