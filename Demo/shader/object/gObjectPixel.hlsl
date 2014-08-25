Texture2D colorTex : register(t0);

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

uint4 pack(int4 a, int4 b, int4 c, int4 d)
{
	uint4 res =
		(0xFF000000 & (a << 24)) |
		(0x00FF0000 & (b << 16)) |
		(0x0000FF00 & (c << 8)) |
		(0x000000FF & d);

	return res;
}

PS_OUTPUT main(PS_INPUT input)
{
	PS_OUTPUT output = (PS_OUTPUT)0;

	float4 color = colorTex.Sample(colorSampleType, input.texCoord);

	if (color.a > 0.5)
	{
		output.normal = float4(normalize(input.normal), matShininess);
		output.material = pack(color * 255, matAmbient * 255, matDiffuse * 255, matSpecular * 255);
		output.depth = input.position.z;
	}
	else
		output.depth = 1.0;

	return output;
}