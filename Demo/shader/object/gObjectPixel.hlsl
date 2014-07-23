Texture2D colorTex : register(t0);
Texture2D normalTex : register(t1);
Texture2D materialTex : register(t2);
Texture2D depthTex : register(t3);

SamplerState colorSampleType : register(s0);
SamplerState gBufferSampleType : register(s1);

cbuffer screenBuffer : register(b0)
{
	float2 screen;
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
	float4 normal : SV_TARGET0;
	uint4 material : SV_TARGET1;
	//float depth : SV_Depth;
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
		//output.depth = input.position.z;
	}
	else
	{
		output.normal = normalTex.Sample(gBufferSampleType, input.position.xy / screen);
		output.material = materialTex.Sample(gBufferSampleType, input.position.xy / screen);
		//output.depth = depthTex.Sample(gBufferSampleType, input.position.xy / screen).x;
	}

	return output;
}