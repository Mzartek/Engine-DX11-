Texture2D<float4> normalTex : register(t0);
Texture2D<uint4> materialTex : register(t1);
Texture2D<float> depthTex : register(t2);
Texture2D<uint4> stencilTex : register(t3);

Texture2D shadowMap0 : register(t4);
Texture2D shadowMap1 : register(t5);
Texture2D shadowMap2 : register(t6);
SamplerComparisonState shadowMapSamplerComparisonState : register(s0);

cbuffer mainInfoBuffer : register (b0)
{
	matrix IVPMatrix;
	uint2 screen;
	vector camPosition;
}

cbuffer LightInfoBuffer : register (b1)
{
	matrix shadowMatrix[3];
	float3 lightColor;
	float3 lightDirection;
	bool withShadowMapping;
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

float lookUp(float4 coord, float2 offSet, int2 texSize, Texture2D shadowMap)
{
	coord.x = 0.5f + (coord.x / coord.w * 0.5f);
	coord.y = 0.5f - (coord.y / coord.w * 0.5f);
	coord.z = (coord.z - 0.0025) / coord.w;

	coord.x += offSet.x * (1.0 / texSize.x);
	coord.y += offSet.y * (1.0 / texSize.y);

	if (coord.x > 1.0 || coord.x < 0.0 || coord.y > 1.0 || coord.y < 0.0)
		return 1.0;
	return shadowMap.SampleCmpLevelZero(shadowMapSamplerComparisonState, coord.xy, coord.z);
}

float calcShadow(float4 coord, float pcf, Texture2D shadowMap)
{
	float a, x, y, shadow = 0.0;
	uint width, height;
	shadowMap.GetDimensions(width, height);

	a = (pcf - 1.0) / 2.0;
	for (x = -a; x <= a; x += 1.0)
		for (y = -a; y <= a; y += 1.0)
			shadow += lookUp(coord, float2(x, y), int2(width, height), shadowMap);
	shadow /= (pcf*pcf);

	return shadow;
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

	if (stencilTex[input.position.xy].y != 1) 
		discard;

	float3 position = getPosition(input.position.xy);
	float4 normal = normalTex[input.position.xy];
	uint4 material = materialTex[input.position.xy];

	float4 diffColor = unpackUnorm4x8(material.z) * float4(lightColor, 1.0);
	float4 specColor = unpackUnorm4x8(material.w) * float4(lightColor, 1.0);

	float3 cam_minus_pos = camPosition - position;
	float shadow = 1.0;
	if (withShadowMapping)
	{
		float distance = length(cam_minus_pos);
		if      (distance < 25) shadow = calcShadow(mul(shadowMatrix[0], float4(position, 1.0)), 3.0, shadowMap0);
		else if (distance < 50) shadow = calcShadow(mul(shadowMatrix[1], float4(position, 1.0)), 1.0, shadowMap1);
		else                    shadow = calcShadow(mul(shadowMatrix[2], float4(position, 1.0)), 1.0, shadowMap2);
	}
	output.light = calcLight(diffColor, specColor, normal.xyz, normalize(-lightDirection), normalize(camPosition.xyz - position), normal.w) * shadow;

	return output;
}
