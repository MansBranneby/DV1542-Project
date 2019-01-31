struct VS_IN
{
	float3 pos : POSITION;
	float2 tex : UV_COORD;
};

struct VS_OUT
{
	float4 pos : SV_POSITION;
	float2 tex : UV_COORD;
};

VS_OUT VS_main(VS_IN input)
{
	VS_OUT output = (VS_OUT)0;

	output.pos = float4(input.pos, 1);
	output.tex = input.tex;

	return output;
}