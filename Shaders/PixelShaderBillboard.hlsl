struct GS_OUT
{
	float4 pos : SV_POSITION;
	float4 worldPos : World_POSITION;
	float4 worldNor : World_NORMAL;
	float2 tex : TEXCOORD;
	float3 col : COLOUR;
};

struct PS_OUT
{
	float4 posWS : SV_Target0;
	float4 norWS : SV_Target1;
	float4 col : SV_Target2;
};

PS_OUT PS_main(GS_OUT input)
{
	PS_OUT output;
	
	output.posWS = input.pos;
	output.norWS = input.worldNor;
	output.col = float4(input.col, 1.0f);

	return output;
};