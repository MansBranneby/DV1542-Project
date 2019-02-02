//Texture2D txDiffuse : register(t0);
//SamplerState sampAni;

struct GS_OUT
{
	float4 pos : SV_POSITION;
	float4 posWS : WORLD_POSITION;
	float4 norWS : WORLD_NORMAL;
	float3 col : COLOUR;
};

struct PS_OUT
{
	float4 posWS : SV_Target0;
	float4 norWS : SV_Target1;
	float3 col : SV_Target2;
};

//cbuffer FS_CONSTANT_BUFFER : register(b0)
//{
//	float3 lightPos;
//	float3 lightCol;
//	float3 cameraPos;
//};

PS_OUT PS_main(GS_OUT input)// : SV_Target
{
	//float3 textureCol = txDiffuse.Sample(sampAni, input.Tex).xyz;
	
	// LIGHTING //

	//Ambient
	//float3 ambientCol = { 0.2, 0.2, 0.2 };
	//float3 ambient = input.col * ambientCol;

	////Diffuse
	//float diffuseFactor = max(dot(normalize(lightPos - input.worldPos.xyz), normalize(input.worldNor.xyz)), 0);
	//float3 diffuse = input.col * lightCol * diffuseFactor;

	////Specular
	//float3 n = normalize(input.worldNor.xyz);
	//float3 l = normalize(lightPos - input.worldPos.xyz);
	//float3 v = normalize(cameraPos - input.worldPos.xyz);
	//float3 r = normalize(2 * dot(n, l) * n - l);
	//
	//float3 specular = input.col * lightCol * pow(max(dot(r, v), 0), 2);

	////Final
	//float3 fragmentCol = ambient + diffuse + specular;
	//return float4(fragmentCol, 1.0f);
	PS_OUT output;
	output.posWS = input.posWS;
	output.norWS = normalize(input.norWS);
	output.col = input.col;

	return output;
};