struct VS_IN
{
	float3 pos : POSITION;
	float2 tex : TEXCOORD;
	float3 col : COLOUR;
};

struct VS_OUT
{
	float4 pos : SV_POSITION;
	float2 tex : TEXCOORD;
	float3 col : COLOUR;
};

VS_OUT VS_main(VS_IN input)
{
	VS_OUT output = (VS_OUT)0;

	output.pos = float4(input.pos, 1.0f);
	output.tex = input.tex;
	output.col = input.col;

	return output;
}