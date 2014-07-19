Texture2D tex : register(t0);
SamplerState sampleType : register(s0);

cbuffer screenBuffer : register(b0)
{
	float screenWidth;
	float screenHeight;
}

cbuffer materialBuffer : register(b1)
{
	float4 matAmbient;
	float4 matDiffuse;
	float4 matSpecular;
}

struct PS_INPUT
{
	float4 position : SV_POSITION;
	float2 texCoord : IN_TEXCOORD;
	float3 normal : IN_NORMAL;
};

float4 main(PS_INPUT input) : SV_TARGET
{
	return tex.Sample(sampleType, input.texCoord) * matDiffuse;
}