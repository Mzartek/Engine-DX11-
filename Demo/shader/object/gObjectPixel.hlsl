Texture2D colorTex : register(t0);
Texture2D<float4> normalTex : register(t1);
Texture2D<uint4> materialTex : register(t2);
Texture2D<float> depthTex : register(t3);

SamplerState colorSampleType : register(s0);

cbuffer materialBuffer : register(b0)
{
	float4 matAmbient;
	float4 matDiffuse;
	float4 matSpecular;
	float matShininess;
}

struct PS_INPUT
{
	float4 position : SV_POSITION;
	float2 texCoord : IN_TEXCOORD;
	float3 normal : IN_NORMAL;
};

struct PS_OUTPUT
{
	float4 normal : SV_TARGET0;
	uint4 material : SV_TARGET1;
	float depth : SV_Depth;
};

PS_OUTPUT main(PS_INPUT input)
{
	PS_OUTPUT output = (PS_OUTPUT)0;

	float4 color = colorTex.Sample(colorSampleType, input.texCoord);

	if (color.a > 0.5)
	{
		output.normal = float4(normalize(input.normal), matShininess);
		output.material =
			uint4(0xFF000000 & uint4(int4(color * 255) << 24)) |
			uint4(0x00FF0000 & int4(matAmbient * 255) << 16) |
			uint4(0x0000FF00 & int4(matDiffuse * 255) << 8) |
			uint4(0x000000FF & int4(matSpecular * 255));
		output.depth = input.position.z;
	}
	else
	{
		output.normal = normalTex[input.position.xy];
		output.material = materialTex[input.position.xy];
		output.depth = depthTex[input.position.xy];
	}

	return output;
}