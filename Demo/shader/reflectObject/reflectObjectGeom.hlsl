struct GS_INPUT
{
	float4 position : SV_POSITION;
	float2 texCoord : IN_TEXCOORD;
	float3x3 TBN : IN_TBN;
	float3 eyeVec : IN_EYEVEC;
};

struct GS_OUTPUT
{
	float4 position : SV_POSITION;
	float2 texCoord : IN_TEXCOORD;
	float3x3 TBN : IN_TBN;
	float3 eyeVec : IN_EYEVEC;
};

[maxvertexcount(3)]
void main(triangle GS_INPUT input[3], inout TriangleStream<GS_OUTPUT> outputStream)
{
	GS_OUTPUT output = (GS_OUTPUT)0;

	for (uint i = 0; i<3; i++)
	{
		output.position = input[i].position;
		output.texCoord = input[i].texCoord;
		output.TBN = input[i].TBN;
		output.eyeVec = input[i].eyeVec;

		outputStream.Append(output);
	}
	outputStream.RestartStrip();
}
