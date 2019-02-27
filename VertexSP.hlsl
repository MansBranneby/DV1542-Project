struct VS_IN
{
	float3 pos : POSITION;
	float2 tex : UV_COORD;
};

//cbuffer VS_CONSTANT_BUFFER : register(b0)
//{
//	matrix lightWVP;
//};

struct VS_OUT
{
	float4 pos : SV_POSITION;
	float2 tex : UV_COORD;
	//float4 lightPos : TEXCOORD3;
};

VS_OUT VS_main(VS_IN input)
{
	VS_OUT output = (VS_OUT)0;

	output.pos = float4(input.pos, 1);
	output.tex = input.tex;
	//output.lightPos = mul(float4(input.pos, 1.0), lightWVP);

	return output;
}