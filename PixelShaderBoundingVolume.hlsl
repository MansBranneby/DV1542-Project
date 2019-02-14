struct PS_IN
{
	float4 posWS : SV_POSITION;
	float3 col : COLOUR;
};

struct PS_OUT
{
	float4 posWS : SV_Target0;
	float4 col : SV_Target1;
};

PS_OUT PS_main(PS_IN input)
{
	PS_OUT output;

	output.posWS = input.posWS;
	output.col = float4(input.col, 1.0f);

	return output;
}