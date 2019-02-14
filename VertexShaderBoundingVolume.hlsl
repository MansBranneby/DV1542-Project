struct VS_IN
{
	float3 pos : POSITION;
	float2 col : COLOUR;
};

struct VS_OUT
{
	float4 posWS : SV_POSITION;
	float2 col : COLOUR;
};



VS_OUT VS_main(VS_IN input)
{
	VS_OUT output;

	output.posWS = float4(input.pos, 1.0f);
	output.col = input.col;

	return output;
}