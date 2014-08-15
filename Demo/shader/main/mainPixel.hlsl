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
	float1 matShininess;
}

struct PS_INPUT
{
	float4 position : SV_POSITION;
	float2 texCoord : IN_TEXCOORD;
	float3 normal : IN_NORMAL;
};

struct PS_OUTPUT
{
	float4 finalColor;
};

PS_OUTPUT main(PS_INPUT input) : SV_TARGET
{
	PS_OUTPUT output = (PS_OUTPUT)0;

	output.finalColor = tex.Sample(sampleType, input.texCoord) * matDiffuse;

	return output;
}