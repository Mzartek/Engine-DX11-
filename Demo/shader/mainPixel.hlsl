Texture2D tex : register(t0);
SamplerState sampleType : register(s0);

struct PS_INPUT
{
	float4 position : SV_POSITION;
	float2 texCoord : IN_TEXCOORD;
	float3 normal : IN_NORMAL;
};

float4 main(PS_INPUT input) : SV_TARGET
{
	return tex.Sample(sampleType, input.texCoord);// +float4(0.5, 0.5, 0.5, 1.0);
}