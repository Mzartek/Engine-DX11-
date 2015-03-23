Texture2D<float4> normalTex : register(t0);
Texture2D<uint4> materialTex : register(t1);
Texture2D<float> depthTex : register(t2);
Texture2D<uint4> stencilTex : register(t3);

cbuffer mainInfoBuffer : register (b0)
{
	matrix IVPMatrix;
	uint2 screen;
	vector camPosition;
}

cbuffer vDirLightInfo : register (b1)
{
	float3 dlColor[20];
	float3 dlDirection[20];
	uint numDirLight;
}

cbuffer vSpotLightInfo : register (b2)
{
	float3 slColor[20];
	float3 slPosition[20];
	float3 slDirection[20];
	float slSpotCutOff[20];
	uint numSpotLight;
}

struct PS_INPUT
{
	float4 position : SV_POSITION;
};

struct PS_OUTPUT
{
	float4 light : SV_TARGET;
};

float4 unpackUnorm4x8(uint v)
{
	float4 res;
	res.x = float(0x000000FF & (v >> 24)) / 255.0;
	res.y = float(0x000000FF & (v >> 16)) / 255.0;
	res.z = float(0x000000FF & (v >> 8)) / 255.0;
	res.w = float(0x000000FF & v) / 255.0;
	return res;
}

float3 getPosition(float2 pixelCoord)
{
	float depth = depthTex[pixelCoord];
	float4 tmp1 = float4(pixelCoord.x / screen.x * 2.0 - 1.0, (1.0 - (pixelCoord.y / screen.y)) * 2.0 - 1.0, depth, 1.0);
	float4 tmp2 = mul(IVPMatrix, tmp1);
	return tmp2.xyz / tmp2.w;
}

float4 calcLight(float4 diffColor, float4 specColor, float3 N, float3 L, float3 V, float shininess)
{
	float3 R = reflect(-L, N);
	float4 diff = max(dot(N, L), 0.0) * diffColor;
	float4 spec = pow(max(dot(R, V), 0.0), shininess) * specColor;
	return diff + spec;
}

PS_OUTPUT main(PS_INPUT input)
{
	PS_OUTPUT output = (PS_OUTPUT)0;

	if (stencilTex[input.position.xy].y != 1) return output;

	float3 position = getPosition(input.position.xy);
	float4 normal = normalTex[input.position.xy];
	uint4 material = materialTex[input.position.xy];

	float4 diffColor = unpackUnorm4x8(material.z);
	float4 specColor = unpackUnorm4x8(material.w);
	
	float3 L, V = normalize(camPosition.xyz - position);
	float cos_cur_angle, cos_outer_cone_angle, cos_inner_cone_angle, cos_inner_minus_outer_angle, spot;
	
	uint i;
	for(i=0; i<numDirLight; i++)
	{
		L = normalize(-dlDirection[i]);
		output.light += calcLight(diffColor * float4(dlColor[i], 1.0), specColor * float4(dlColor[i], 1.0),	normal.xyz, L, V, normal.w);
	}
	for(i=0; i<numSpotLight; i++)
	{
		L = normalize(slPosition[i] - position);
		cos_cur_angle = dot(-L, normalize(slDirection[i]));
		cos_outer_cone_angle = cos(radians(slSpotCutOff[i]));
		cos_inner_cone_angle = cos_outer_cone_angle + 0.01;
		cos_inner_minus_outer_angle = cos_inner_cone_angle - cos_outer_cone_angle;
		spot = clamp((cos_cur_angle - cos_outer_cone_angle) / cos_inner_minus_outer_angle, 0.0, 1.0);
		output.light += calcLight(diffColor * float4(dlColor[i], 1.0), specColor * float4(dlColor[i], 1.0),	normal.xyz, L, V, normal.w) * spot;
	}

	return output;
}
