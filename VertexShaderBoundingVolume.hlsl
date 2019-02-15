struct VS_IN
{
	float3 pos : POSITION;
	float3 col : COLOUR;
};

struct VS_OUT
{
	float4 posWS : SV_POSITION;
	float3 col : COLOUR;
};

cbuffer GS_CONSTANT_BUFFER : register(b0)
{
	matrix world, worldViewProj;
};

VS_OUT VS_main(VS_IN input)
{
	VS_OUT output;

	output.posWS = mul(float4(input.pos, 1.0f), worldViewProj);
	output.col = input.col;

	return output;
}