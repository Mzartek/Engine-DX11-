struct GS_INPUT
{
	float4 position : SV_POSITION;
	float2 texCoord : IN_TEXCOORD;
	float3 normal : IN_NORMAL;
	float3 tangent : IN_TANGENT;
}; 

struct GS_OUTPUT
{
	float4 position : SV_POSITION;
	float2 texCoord : IN_TEXCOORD;
	float3 normal : IN_NORMAL;
	float3 tangent : IN_TANGENT;
};

[maxvertexcount(3)]
void main(triangle GS_INPUT input[3], inout TriangleStream<GS_OUTPUT> outputStream)
{
	GS_OUTPUT output = (GS_OUTPUT)0;
	
	for (uint i = 0; i<3; i++)
	{
		output.position = input[i].position;
		output.texCoord = input[i].texCoord;
		output.normal = input[i].normal;
		output.tangent = input[i].tangent;
		
		outputStream.Append(output);
	}
	outputStream.RestartStrip();
}