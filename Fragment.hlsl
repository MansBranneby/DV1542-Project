Texture2D txDiffuse : register(t0);
SamplerState sampAni;

struct GS_OUT
{
	float4 pos : SV_POSITION;
	float4 worldPos : World_POSITION;
	float4 worldNor : World_NORMAL;
	float2 tex : TEXCOORD;
	float3 col : COLOUR;
};

cbuffer FS_CONSTANT_BUFFER : register(b0)
{
	float3 lightPos;
	float3 lightCol;
};

float4 PS_main(GS_OUT input) : SV_Target
{
	float3 textureCol = txDiffuse.Sample(sampAni, input.tex).xyz;
	float3 ambientCol = { 1.0, 1.0, 1.0 };
	float3 fragmentCol = textureCol * ambientCol;
	//float3 fragmentCol = input.col * ambientCol;
	float diffuseFactor = max(dot(normalize(lightPos - input.worldPos.xyz), normalize(input.worldNor.xyz)), 0);
	fragmentCol += input.col * diffuseFactor * lightCol;
	return float4(fragmentCol, 1.0f);
};