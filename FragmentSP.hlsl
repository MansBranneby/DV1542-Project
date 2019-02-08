Texture2D txPosition : register(t0);
Texture2D txNormal : register(t1);
Texture2D txColour : register(t2);
SamplerState sampAni;

struct VS_OUT
{
	float4 pos : SV_POSITION;
	float2 tex : UV_COORD;
};

cbuffer FS_CONSTANT_BUFFER_LIGHT : register(b0)
{
	float3 lightPos;
	float3 lightCol;
};
cbuffer FS_CONSTANT_BUFFER_CAMERA : register(b1)
{
	float3 cameraPos;
};

float4 PS_main(VS_OUT input) : SV_Target
{
	float3 position = txPosition.Sample(sampAni, input.tex).xyz;
	float3 normal = txNormal.Sample(sampAni, input.tex).xyz;
	float3 colour = txColour.Sample(sampAni, input.tex).xyz;

	//LIGHTING//

	//Ambient
	float3 ambientCol = { 0.5, 0.5, 0.5 };
	float3 ambient = colour * ambientCol;

	//Diffuse
	float diffuseFactor = max(dot(normalize(lightPos - position), normalize(normal)), 0);
	float3 diffuse = colour * lightCol * diffuseFactor;

	//Specular
	float3 n = normalize(normal);
	float3 l = normalize(lightPos - position);
	float3 v = normalize(cameraPos - position);
	float3 r = normalize(2 * dot(n, l) * n - l);

	float3 specular = colour * lightCol * pow(max(dot(r, v), 0), 20);

	//Final
	float3 fragmentCol = ambient + diffuse + specular;

	return float4(fragmentCol, 1.0f);
};