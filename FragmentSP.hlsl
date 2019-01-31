Texture2D txPosition : register(t0);
SamplerState sampAni;

struct VS_OUT
{
	float4 pos : SV_POSITION;
	float2 tex : UV_COORD;
};

cbuffer FS_CONSTANT_BUFFER : register(b0)
{
	float3 lightPos;
	float3 lightCol;
	float3 cameraPos;
};

float4 PS_main(VS_OUT input) : SV_Target
{
	float3 textureCol = txPosition.Sample(sampAni, input.tex).xyz;

	//LIGHTING//

	//Ambient
//	float3 ambientCol = { 0.2, 0.2, 0.2 };
//float3 ambient = input.col * ambientCol;
//
////Diffuse
//float diffuseFactor = max(dot(normalize(lightPos - input.worldPos.xyz), normalize(input.worldNor.xyz)), 0);
//float3 diffuse = input.col * lightCol * diffuseFactor;
//
////Specular
//float3 n = normalize(input.worldNor.xyz);
//float3 l = normalize(lightPos - input.worldPos.xyz);
//float3 v = normalize(cameraPos - input.worldPos.xyz);
//float3 r = normalize(2 * dot(n, l) * n - l);
//
//float3 specular = input.col * lightCol * pow(max(dot(r, v), 0), 2);
//
////Final
//float3 fragmentCol = ambient + diffuse + specular;
//return float4(fragmentCol, 1.0f);
	return float4(textureCol, 1.0);
};