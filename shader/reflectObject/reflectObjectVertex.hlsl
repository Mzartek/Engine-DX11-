cbuffer matrixBuffer : register(b0)
{
	matrix MVP;
	matrix projectionMatrix;
	matrix viewMatrix;
	matrix modelMatrix;
	matrix normalMatrix;
}

cbuffer cameraBuffer : register(b1)
{
	vector camPosition;
	vector camTarget;
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
	float3 eyeVec : IN_EYEVEC;
};

VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	output.position = mul(MVP, float4(input.position, 1.0));
	output.texCoord = input.texCoord;
	output.normal = normalize(mul(normalMatrix, float4(input.normal, 0.0)).xyz);
	output.tangent = normalize(mul(normalMatrix, float4(input.tangent, 0.0)).xyz);
	output.bitangent = cross(output.normal, output.tangent);
	output.eyeVec = mul(modelMatrix, float4(input.position, 1.0)).xyz - camPosition.xyz;

	return output;
}
