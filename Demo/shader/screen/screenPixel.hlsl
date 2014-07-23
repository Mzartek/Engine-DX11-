Texture2D materialTex : register(t0);

SamplerState gBufferSampleType : register(s0);

cbuffer screenBuffer : register(b0)
{
	float2 screen;
}

cbuffer colorBuffer : register(b1)
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

	output.finalColor = materialTex.Sample(gBufferSampleType, input.position.xy / screen);
	//output.finalColor = float4(0x000000FF & (int4(material) >> 16)) / 255;

	return output;
}