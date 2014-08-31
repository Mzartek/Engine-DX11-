cbuffer MVPMatrixBuffer : register(b0)
{
	matrix MVP;
}

cbuffer NormalMatrixBuffer : register(b1)
{
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
	float3 normal : IN_NORMAL;
	float3 tangent : IN_TANGENT;
};

VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	output.position = mul(MVP, float4(input.position, 1.0));
	output.texCoord = input.texCoord;
	output.normal = normalize(input.normal); //normalize(mul(normalMatrix, float4(input.normal, 1.0)).xyz)
	output.tangent = normalize(mul(normalMatrix, float4(input.tangent, 1.0)).xyz);

	return output;
}