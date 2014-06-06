cbuffer materialBuffer : register(b0)
{
	float4 matAmbient;
	float4 matDiffuse;
	float4 matSpecular;
	float matShininess;
}

cbuffer matrixBuffer : register(b1)
{
	float screenWidth;
	float screenHeight;
	matrix MVP;
}

struct VS_INPUT
{
	float4 position : IN_POSITION;
	float2 texCoord : IN_TEXCOORD;
	float3 normal : IN_NORMAL;
};

struct PS_OUTPUT
{
	float4 position : SV_POSITION;
	float2 texCoord : IN_TEXCOORD;
	float3 normal : IN_NORMAL;
};

PS_OUTPUT main(VS_INPUT input)
{
	PS_OUTPUT output = (PS_OUTPUT)0;
	output.position = mul(input.position, MVP);
	output.texCoord = input.texCoord;
	return output;
}