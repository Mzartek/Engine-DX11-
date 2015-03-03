Texture2D<uint4> materialTex : register(t0);
Texture2D<float4> lightTex : register(t1);

struct PS_INPUT
{
	float4 position : SV_POSITION;
};

struct PS_OUTPUT
{
	float4 finalColor : SV_TARGET;
};

float4 unpackUnorm4x8(uint v)
{
	float4 res;
	res.x = float(0x000000FF & (v >> 24)) / 255.0;
	res.y = float(0x000000FF & (v >> 16)) / 255.0;
	res.z = float(0x000000FF & (v >> 8)) / 255.0;
	res.w = float(0x000000FF & v) / 255.0;
	return res;
}

PS_OUTPUT main(PS_INPUT input)
{
	PS_OUTPUT output = (PS_OUTPUT)0;

	uint4 material = materialTex[input.position.xy];
		float4 light = lightTex[input.position.xy];

		output.finalColor = unpackUnorm4x8(material.x) * (unpackUnorm4x8(material.y) + light);
	output.finalColor = clamp(output.finalColor, 0.0, 1.0);

	return output;
}
