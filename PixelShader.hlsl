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
	float4 amb : SV_Target3;
	float4 dif : SV_Target4;
	float4 spec : SV_Target5;
};

cbuffer PS_CONSTANT_BUFFER : register(b0)
{
	float4 ambient;
	float4 diffuse;
	float4 specular;
	float4 specularExp;
};

PS_OUT PS_main(GS_OUT input)
{	
	PS_OUT output;

	output.posWS = input.posWS;
	output.norWS = normalize(input.norWS);
	output.col = float4(txDiffuse.Sample(sampAni, input.tex).xyz, 1.0);
	output.amb = float4(ambient.xyz, 1.0f);
	output.dif = float4(diffuse.xyz, 1.0f);
	output.spec = float4(specular.xyz, specularExp.x);

	return output;
};