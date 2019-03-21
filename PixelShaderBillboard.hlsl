struct GS_OUT
{
	float4 pos : SV_POSITION;
	float4 worldPos : World_POSITION;
	float4 worldNor : World_NORMAL;
	float2 tex : TEXCOORD;
	float3 col : COLOUR;
};

float4 PS_main(GS_OUT input) : SV_Target
{
	return float4(input.col, 1.0f);
};