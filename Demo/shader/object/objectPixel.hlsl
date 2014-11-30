Texture2D colorTex : register(t0);
Texture2D NMTex : register(t1);
SamplerState colorSampleType : register(s0);
SamplerState NMSampleType : register(s1);

cbuffer materialBuffer : register(b0)
{
	float4 matAmbient;
	float4 matDiffuse;
	float4 matSpecular;
	float shininess;
}

struct PS_INPUT
{
	float4 position : SV_POSITION;
	float2 texCoord : IN_TEXCOORD;
	float3x3 TBN : IN_TBN;
};

struct PS_OUTPUT
{
	float4 normal : SV_TARGET0;
	uint4 material : SV_TARGET1;
	float depth : SV_Depth;
};

uint packUnorm4x8(float4 v)
{
	uint res;
	res = uint(round(clamp(v.x, 0.0, 1.0) * 255.0)) << 24;
	res |= uint(round(clamp(v.y, 0.0, 1.0) * 255.0)) << 16;
	res |= uint(round(clamp(v.z, 0.0, 1.0) * 255.0)) << 8;
	res |= uint(round(clamp(v.w, 0.0, 1.0) * 255.0));
	return res;
}

float3 CalcBumpedNormal(float3x3 TBN, float3 bumpMapNormal)
{
	return normalize(mul(bumpMapNormal* 2.0 - 1.0, TBN));
}

PS_OUTPUT main(PS_INPUT input)
{
	PS_OUTPUT output = (PS_OUTPUT)0;

	float4 color = colorTex.Sample(colorSampleType, input.texCoord);
	float3 normal = CalcBumpedNormal(input.TBN, NMTex.Sample(NMSampleType, input.texCoord).xyz);

	if (color.a > 0.5)
	{
		output.normal = float4(normal, shininess);
		output.material.x = packUnorm4x8(color);
		output.material.y = packUnorm4x8(matAmbient);
		output.material.z = packUnorm4x8(matDiffuse);
		output.material.w = packUnorm4x8(matSpecular);
		output.depth = input.position.z;
	}
	else
		discard;

	return output;
}