cbuffer matrixBuffer : register(b0)
{
	matrix MVP;
	matrix modelView;
	matrix normalMatrix;
}

struct VS_INPUT
{
	float3 position : IN_POSITION;
	float2 texCoord : IN_TEXCOORD;
	float3 normal : IN_NORMAL;
};

struct VS_OUTPUT
{
	float4 position : SV_POSITION;
	float2 texCoord : IN_TEXCOORD;
	float3 normal : IN_NORMAL;
};

VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	output.position = mul(float4(input.position, 1.0), MVP);
	output.texCoord = input.texCoord;
	output.normal = mul(float4(input.normal, 1.0), normalMatrix).xyz;

	return output;
}