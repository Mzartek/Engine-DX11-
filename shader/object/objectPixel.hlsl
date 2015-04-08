Texture2D diffuseTex : register(t0);
Texture2D specularTex : register(t1);
Texture2D ambientTex : register(t2);
Texture2D emissiveTex : register(t3);
Texture2D shininessTex : register(t4);
Texture2D opacityTex : register(t5);
Texture2D bumpMap : register(t6);
Texture2D normalMap : register(t7);
Texture2D displacementMap : register(t8);
Texture2D lightMap : register(t9);

SamplerState diffuseSampleType : register(s0);
SamplerState specularSampleType : register(s1);
SamplerState ambientSampleType : register(s2);
SamplerState emissiveSampleType : register(s3);
SamplerState shininessSampleType : register(s4);
SamplerState opacitySampleType : register(s5);
SamplerState bumpSampleType : register(s6);
SamplerState normalSampleType : register(s7);
SamplerState displacementSampleType : register(s8);
SamplerState lightSampleType : register(s9);

cbuffer materialBuffer : register(b0)
{
	float3 matDiffuse;
	float3 matSpecular;
	float3 matAmbient;
	float3 matEmissive;
	float matShininess;
	float matOpacity;
}

cbuffer stateBuffer : register(b1)
{
	bool hasDiffuseTexture;
	bool hasSpecularTexture;
	bool hasAmbientTexture;
	bool hasEmissiveTexture;
	bool hasShininessTexture;
	bool hasOpacityTexture;
	bool hasBumpMap;
	bool hasNormalMap;
	bool hasDisplacementMap;
	bool hasLightMap;
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

	float4 color = diffuseTex.Sample(diffuseSampleType, input.texCoord);
	float3 normal = CalcBumpedNormal(input.TBN, normalMap.Sample(normalSampleType, input.texCoord).xyz);

	if (color.a > 0.5)
	{
		output.normal = float4(normal, matShininess);
		output.material.x = packUnorm4x8(color);
		output.material.y = packUnorm4x8(float4(matAmbient, matOpacity));
		output.material.z = packUnorm4x8(float4(matDiffuse, matOpacity));
		output.material.w = packUnorm4x8(float4(matSpecular, matOpacity));
		output.depth = input.position.z;
	}
	else
		discard;

	return output;
}
