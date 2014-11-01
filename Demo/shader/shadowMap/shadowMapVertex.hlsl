cbuffer MVPMatrixBuffer : register(b0)
{
	matrix MVP;
	matrix normalMatrix;
}

struct VS_INPUT
{
	float3 position : IN_POSITION;
	float2 texCoord : IN_TEXCOORD;
	float3 normal : IN_NORMAL;
	float3 tangent : IN_TANGENT;
};

struct VS_OUTPUT
{
	float4 position : SV_POSITION;
	float2 texCoord : IN_TEXCOORD;
};

VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	output.position = mul(MVP, float4(input.position, 1.0));
	output.texCoord = input.texCoord;

	return output;
}
