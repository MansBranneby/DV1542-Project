Texture2D txDiffuse : register(t0);
SamplerState sampAni;

struct GS_OUT
{
	float4 pos : SV_POSITION;
	float4 posWS : WORLD_POSITION;
	float4 norWS : WORLD_NORMAL;
	float2 tex : TEXCOORD;
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

	output.posWS = input.posWS;
	output.norWS = normalize(input.norWS);
	output.col = float4(txDiffuse.Sample(sampAni, input.tex).xyz, 1.0);

	return output;
};