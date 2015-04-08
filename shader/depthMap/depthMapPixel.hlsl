Texture2D diffuseTex : register(t0);
SamplerState diffuseSampleType : register(s0);

struct PS_INPUT
{
	float4 position : SV_POSITION;
	float2 texCoord : IN_TEXCOORD;
};

struct PS_OUTPUT
{
	float depth : SV_Depth;
};

PS_OUTPUT main(PS_INPUT input)
{
	PS_OUTPUT output = (PS_OUTPUT)0;

	float transparency = diffuseTex.Sample(diffuseSampleType, input.texCoord).w;
	if (transparency > 0.5)
		output.depth = input.position.z;
	else
		discard;

	return output;
}
