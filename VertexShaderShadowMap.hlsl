struct VS_IN
{
	float3 pos : POSITION;
	float2 tex : TEXCOORD;
	float3 col : COLOUR;
};

struct VS_OUT
{
	float4 pos : SV_POSITION;
	float4 posWS : POSITION;
};

cbuffer GS_CONSTANT_BUFFER : register(b0)
{
	matrix world, worldViewProj;
};

cbuffer VS_CONSTANT_BUFFER_SM : register(b1)
{
	matrix lightWVP;
};

VS_OUT VS_main(VS_IN input)
{
	VS_OUT output = (VS_OUT)0;

	output.pos = mul(float4(input.pos, 1.0f), worldViewProj);
	output.posWS = mul(float4(input.pos, 1.0f), world);
	return output;
}