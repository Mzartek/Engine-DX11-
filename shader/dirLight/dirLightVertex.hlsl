struct VS_INPUT
{
	float2 position : IN_POSITION;
};

struct VS_OUTPUT
{
	float4 position : SV_POSITION;
};

VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	output.position = float4(input.position, 0.0, 1.0);

	return output;
}
