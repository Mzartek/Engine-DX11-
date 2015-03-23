cbuffer matrixBuffer : register(b0)
{
	matrix MVP;
}

struct VS_INPUT
{
	float3 position : IN_POSITION;
};

struct VS_OUTPUT
{
	float4 position : SV_POSITION;
	float3 texCoord : IN_TEXCOORD;
};

VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	output.position = mul(MVP, float4(input.position, 1.0));
	output.texCoord = float3(input.position.x, -input.position.y, input.position.z);

	return output;
}
