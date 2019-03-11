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
};

struct PS_OUT
{
	float4 posWS : SV_Target0;
	float4 norWS : SV_Target1;
	float4 col : SV_Target2;
	float4 amb : SV_Target3;
	float4 dif : SV_Target4;
	float4 spec : SV_Target5;
};

cbuffer PS_CONSTANT_BUFFER : register(b0)
{
	float3 ambient;
	float3 diffuse;
	float3 specular;
	float specularExp;
};

PS_OUT PS_main(PS_IN input)
{
	PS_OUT output;

	input.tanWS = normalize(input.tanWS - dot(input.tanWS, input.norWS)*input.norWS);
	float3 biTanWS = cross(input.norWS, input.tanWS);

	float3x3 tangentToWS = float3x3(input.tanWS, biTanWS, input.norWS);
	float3 normalWS = mul(normalize(float3(txNormal.Sample(sampAni, input.tex).xyz) * 2.0f - 1.0f), tangentToWS);

	output.posWS = input.posWS;
	output.norWS = float4(normalWS, 1.0f);
	output.col = float4(txDiffuse.Sample(sampAni, input.tex).xyz, 1.0);
	output.amb = float4(ambient, 1.0f);
	output.dif = float4(diffuse, 1.0f);
	output.spec = float4(specular, specularExp);

	return output;
};