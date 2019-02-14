struct VS_OUT
{
	float4 pos : SV_POSITION;
	float4 posWS : POSITION;
};

struct PS_OUT
{
	float4 posWS : SV_Target0;
};

PS_OUT PS_main(VS_OUT input)
{
	PS_OUT output;
	output.posWS = float4(1.0, 0.0, 0.0, 1.0);
	return output;
}