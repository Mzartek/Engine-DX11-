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
	float3x3 TBN : IN_TBN;
};

VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	float3 N = mul(normalMatrix, input.normal);
	float3 T = mul(normalMatrix, input.tangent);
	float3 B = cross(N, T);

	output.position = mul(MVP, float4(input.position, 1.0));
	output.texCoord = input.texCoord;
	output.TBN = float3x3(T, B, N);

	return output;
}