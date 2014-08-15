Texture2D<uint4> materialTex : register(t0);

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

	uint4 material = materialTex[input.position.xy];

	output.finalColor = float4(0x000000FF & (int4(material) >> 24)) / 255;
	output.finalColor *= color;

	return output;
}