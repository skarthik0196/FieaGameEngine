cbuffer CBufferPerObject
{
	float4x4 WorldViewProjectionMatrix :WORLDVIEWPROJECTION;
	float4x4 WorldMatrix : WORLDMATRIX;
};

struct VS_INPUT
{
	float4 ObjectPosition : POSITION;
	float2 TextureCoordinates :TEXTURECOORDINATE;
	float3 Normal : NORMAL;
};

struct VS_OUTPUT
{
	float4 Position : SV_POSITION;
	float2 TextureCoordinate : TEXTURECOORDINATE;
};

VS_OUTPUT main(VS_INPUT IN)
{

	VS_OUTPUT OUT = (VS_OUTPUT)0;

	OUT.Position = mul(IN.ObjectPosition, WorldViewProjectionMatrix);
	OUT.TextureCoordinate = IN.TextureCoordinates;

	return OUT;
}