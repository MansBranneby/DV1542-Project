struct VS_IN
{
	float3 pos : POSITION;
	float2 tex : TEXCOORD;
	float3 col : COLOUR;
};

cbuffer VS_CONSTANT_BUFFER : register(b0)
{
	matrix lightWVP;
};

struct VS_OUT
{
	float4 pos : SV_POSITION;
};

VS_OUT VS_main(VS_IN input)
{
	VS_OUT output = (VS_OUT)0;
	output.pos = mul(float4(input.pos, 1.0f), lightWVP);
	return output;
}