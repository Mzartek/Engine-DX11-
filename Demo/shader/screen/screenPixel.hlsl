Texture2D<uint4> materialTex : register(t0);
Texture2D<float4> lightTex : register(t1);

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

int4 unpack(uint4 a, int v)
{
	return (0x000000FF & (a >> (v * 8)));
}

PS_OUTPUT main(PS_INPUT input)
{
	PS_OUTPUT output = (PS_OUTPUT)0;

	uint4 material = materialTex[input.position.xy];
	float4 light = lightTex[input.position.xy];

	output.finalColor = (float4(unpack(material, 3)) / 255);
	output.finalColor *= (float4(unpack(material, 2)) / 255) + light;
	output.finalColor *= color;

	return output;
}