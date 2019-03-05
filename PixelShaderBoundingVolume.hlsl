struct PS_IN
{
	float4 posWS : SV_POSITION;
	float3 col : COLOUR;
};

struct PS_OUT
{
	float4 posWS : SV_Target0;
	float4 col : SV_Target2;
};

float4 PS_main(PS_IN input) : SV_Target
{
	PS_OUT output;

	output.posWS = input.posWS;
	output.col = float4(input.col, 1.0f); 

	return float4(input.col, 1.0f);
}