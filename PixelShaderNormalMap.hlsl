Texture2D txDiffuse : register(t0);
Texture2D txNormal : register(t1);
SamplerState sampAni;

struct PS_IN
{
	float4 pos : SV_POSITION;
	float4 posWS : POSITION;
	float2 tex : TEXCOORD;
	float3 norWS : NORMAL;
	float3 tanWS : TANGENT;
	float3 biTanWS : BI_TANGENT;
};

struct PS_OUT
{
	float4 posWS : SV_Target0;
	float4 norWS : SV_Target1;
	float4 col : SV_Target2;
};

PS_OUT PS_main(PS_IN input)
{
	PS_OUT output;

	float3x3 tangentToWS = float3x3(input.tanWS, input.biTanWS, input.norWS); //transponerad?
	float3 normalWS = mul(normalize(float3(txNormal.Sample(sampAni, input.tex).xyz) * 2.0f - 1.0f), tangentToWS);
	//normalWS.z *= -1.0f; //Beh�vs detta?

	output.posWS = input.posWS;
	output.norWS = float4(normalWS, 1.0f);
	output.col = float4(txDiffuse.Sample(sampAni, input.tex).xyz, 1.0);

	return output;
};