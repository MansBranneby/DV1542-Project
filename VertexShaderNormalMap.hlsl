struct VS_IN
{
	float3 pos : POSITION;
	float2 tex : TEXCOORD;
	float3 nor : NORMAL;
	float3 tan : TANGENT;
};

struct VS_OUT
{
	float4 pos : SV_POSITION;
	float4 posWS : POSITION;
	float2 tex : TEXCOORD;
	float3 norWS : NORMAL;
	float3 tanWS : TANGENT;
};

cbuffer VS_CONSTANT_BUFFER : register(b0)
{
	matrix world, worldViewProj;
};

VS_OUT VS_main(VS_IN input)
{
	VS_OUT output = (VS_OUT)0;

	output.pos = mul(float4(input.pos, 1.0f), worldViewProj);
	output.posWS = mul(float4(input.pos, 1.0f), world);
	output.tex = input.tex;
	output.norWS = normalize(mul(float4(input.nor, 1.0f), world)).xyz;
	output.tanWS = mul(float4(input.tan, 1.0f), world).xyz;

	return output;
}
