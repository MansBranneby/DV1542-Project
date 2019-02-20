struct VS_IN
{
	float3 pos : POSITION;
	float2 tex : TEXCOORD;
	float3 nor : NORMAL;
	float3 tan : TANGENT;
	float3 biTan : BI_TANGENT;
};

struct VS_OUT
{
	float4 pos : SV_POSITION;
	float4 posWS : POSITION;
	float2 tex : TEXCOORD;
	float3 norWS : NORMAL;
	float3 tanWS : TANGENT;
	float3 biTanWS : BI_TANGENT;
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
	output.norWS = normalize(mul(input.nor, world));
	output.tanWS = normalize(mul(input.tan, world));
	output.biTanWS = normalize(mul(input.biTan, world));

	return output;
}
