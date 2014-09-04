cbuffer MVPMatrixBuffer : register(b0)
{
	matrix MVP;
}

cbuffer NormalMatrixBuffer : register(b1)
{
	float3x3 normalMatrix;
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
	float3 bitangent : IN_BITANGENT;
};

VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	output.position = mul(MVP, float4(input.position, 1.0));
	output.texCoord = input.texCoord;
	output.normal = normalize(mul(normalMatrix, input.normal));
	output.tangent = normalize(mul(normalMatrix, input.tangent));
	output.bitangent = cross(output.normal, output.tangent);

	return output;
}