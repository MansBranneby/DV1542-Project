Texture2D txPosition : register(t0);
Texture2D txNormal : register(t1);
Texture2D txColour : register(t2);
Texture2D shadowMap : register(t3);
SamplerState sampAni;

struct VS_OUT
{
	float4 pos : SV_POSITION;
	float2 tex : UV_COORD;
//	float4 lightPos : TEXCOORD3;
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

cbuffer VS_CONSTANT_BUFFER : register(b2)
{
	matrix lightWVP;
};

float4 PS_main(VS_OUT input) : SV_Target
{
	float3 position = txPosition.Sample(sampAni, input.tex).xyz;
	float3 normal = txNormal.Sample(sampAni, input.tex).xyz;
	float3 colour = txColour.Sample(sampAni, input.tex).xyz;
	float4 lightPosition = mul(float4(position, 1.0f), lightWVP);

	//Shadowmap
	float2 shadowTexCoords;
	float shadowMapDepth;
	float pixelDepth;
	shadowTexCoords.x = 0.5f + (lightPosition.x / lightPosition.w * 0.5f);
	shadowTexCoords.y = 0.5f - (lightPosition.y / lightPosition.w * 0.5f);

	pixelDepth = (lightPosition.z / lightPosition.w) -0.001f;
	shadowMapDepth = shadowMap.Sample(sampAni, shadowTexCoords).x;

	bool inLightFrustum = ((saturate(shadowTexCoords.x) == shadowTexCoords.x) && (saturate(shadowTexCoords.y) == shadowTexCoords.y) && (pixelDepth > 0));

	//LIGHTING//

	//Ambient
	float3 ambientCol = { 0.2, 0.2, 0.2 };
	float3 ambient = colour * ambientCol;
	float3 fragmentCol;

	if (pixelDepth < shadowMapDepth || !inLightFrustum)
	{
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
		fragmentCol = ambient + diffuse + specular;
	}
	else
		fragmentCol = ambient;

	return float4(fragmentCol, 1.0f);
};