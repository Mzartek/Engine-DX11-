cbuffer lightPositionBuffer : register(b0)
{
	float3 lightPos;
}

struct GS_INPUT
{
	float4 position : SV_POSITION;
};

struct GS_OUTPUT
{
	float4 position : SV_POSITION;
};

[maxvertexcount(18)]
void main(triangleadj GS_INPUT input[6], inout TriangleStream<GS_OUTPUT> outputStream)
{
	GS_OUTPUT output = (GS_OUTPUT)0;

	for (uint i = 0; i<3; i++)
	{
		output.position = input[i].position;
		outputStream.Append(output);
	}
	outputStream.RestartStrip();
}