TextureCube cubeTex : register(t0);
SamplerState cubeSamplerState : register(s0);

struct PS_INPUT
{
	float4 position : SV_POSITION;
	float3 texCoord : IN_TEXCOORD;
};

struct PS_OUTPUT
{
	float4 normal : SV_TARGET0;
	uint4 material : SV_TARGET1;
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

	output.normal = float4(0.0, 0.0, 0.0, 0.0);
	output.material = pack(cubeTex.Sample(cubeSamplerState, input.texCoord) * 255, int4(255, 255, 255, 255), int4(0, 0, 0, 0), int4(0, 0, 0, 0));

	return output;
}
