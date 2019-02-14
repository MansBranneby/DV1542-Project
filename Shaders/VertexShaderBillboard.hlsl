struct VS_IN
{
	float3 pos : POSITION;
	float3 col : COLOUR;
};

struct VS_OUT
{
	float4 pos : SV_POSITION;
	float3 col : COLOUR;
};
//-----------------------------------------------------------------------------------------
// VertexShader: VSScene
//-----------------------------------------------------------------------------------------
VS_OUT VS_main(VS_IN input)
{
	VS_OUT output = (VS_OUT)0;

	output.pos = float4(input.pos, 1);
	output.col = input.col;
	output.col = float3(1.0f, 1.0f, 1.0f);

	return output;
}