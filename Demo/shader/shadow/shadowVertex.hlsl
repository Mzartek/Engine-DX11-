cbuffer MVPMatrixBuffer : register(b0)
{
	matrix MVP;
}

struct VS_INPUT
{
	float3 position : IN_POSITION;
	float2 texCoord : IN_TEXCOORD;
};

struct VS_OUTPUT
{
	float4 position : SV_POSITION;
	float2 texCoord : IN_TEXCOORD;
};

VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	output.position = mul(float4(input.position, 1.0), MVP);
	output.texCoord = input.texCoord;

	return output;
}
