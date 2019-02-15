Texture2D txPosition : register(t0);
Texture2D txNormal : register(t1);
Texture2D txColour : register(t2);
Texture2D shadowMap : register(t3);
SamplerState sampAni;

struct VS_OUT
{
	float4 pos : SV_POSITION;
	float2 tex : UV_COORD;
	float4 lightPos : TEXCOORD3;
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

	//Shadow map
	input.lightPos.xy /= input.lightPos.w; // now in NDC
	float2 smTex = float2(0.5f*input.lightPos.x + 0.5f, -0.5f*input.lightPos.y + 0.5f); //From [-1,1] to [0,1]
	float depth = input.lightPos.z / input.lightPos.w; //depth

	float dx = 1.0f / 768.0f; // size of one texture sample in the shadow map (width==height)
	float s0 = (shadowMap.Sample(sampAni, smTex).r + 0.001f < depth) ? 0.0f : 1.0f;
	float s1 = (shadowMap.Sample(sampAni, smTex + float2(dx, 0.0f)).r + 0.001f < depth) ? 0.0f : 1.0f;
	float s2 = (shadowMap.Sample(sampAni, smTex + float2(0.0f, dx)).r + 0.001f < depth) ? 0.0f : 1.0f;
	float s3 = (shadowMap.Sample(sampAni, smTex + float2(dx, dx)).r + 0.001f < depth) ? 0.0f : 1.0f;	// Transform shadow map UV coord to texel space
	float2 texelPos = smTex * 768.0f;
	// Determine the lerp amount
	float2 lerps = frac(texelPos);
	float shadowCoeff = lerp(lerp(s0, s1, lerps.x), lerp(s2, s3, lerps.x), lerps.y);
	float3 colour = txColour.Sample(sampAni, input.tex).xyz;// *shadowCoeff;

	//LIGHTING//

	//Ambient
	float3 ambientCol = { 0.1, 0.1, 0.1 };
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