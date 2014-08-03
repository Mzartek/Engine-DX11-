TextureCube cubeTex : register(t0);
Texture2D<float> depthTex : register(t1);

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
	float depth : SV_DEPTH;
};

PS_OUTPUT main(PS_INPUT input)
{
	PS_OUTPUT output = (PS_OUTPUT)0;

	output.normal = float4(0.0, 0.0, 0.0, 0.0);
	output.material =
		uint4(0xFF000000 & uint4(int4(cubeTex.Sample(cubeSamplerState, input.texCoord) * 255) << 24)) |
		uint4(0x00FF0000 & (int4(255, 255, 255, 255) << 16)) |
		uint4(0x0000FFFF & int4(0.0, 0.0, 0.0, 0.0));
	output.depth = input.position.z;

	return output;
}
