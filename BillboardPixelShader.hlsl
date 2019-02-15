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
	float4 colWS : SV_Target2;
};

PS_OUT PS_main(GS_OUT input)
{

	//float3 textureCol = txDiffuse.Sample(sampAni, input.tex).xyz;
	//float3 ambientCol = { 1.0, 1.0, 1.0 };
	//float3 fragmentCol = textureCol * ambientCol;
	////float3 fragmentCol = input.col * ambientCol;
	//float diffuseFactor = max(dot(normalize(lightPos - input.worldPos.xyz), normalize(input.worldNor.xyz)), 0);
	//fragmentCol += input.col * diffuseFactor * lightCol;
	//return float4(fragmentCol, 1.0f);

	PS_OUT output;
	output.posWS = input.pos;
	output.norWS = input.worldNor;
	output.colWS = float4(input.col, 1.0);

	return output;
};