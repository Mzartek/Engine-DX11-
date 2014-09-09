Texture2D backgroundTex : register(t0);

cbuffer screenColorBuffer : register(b0)
{
	float4 color;
}

struct PS_INPUT
{
	float4 position : SV_POSITION;
};

struct PS_OUTPUT
{
	float4 finalColor : SV_TARGET;
};

PS_OUTPUT main(PS_INPUT input)
{
	PS_OUTPUT output = (PS_OUTPUT)0;

	output.finalColor = backgroundTex[input.position.xy] * color;

	return output;
}