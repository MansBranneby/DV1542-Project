struct GS_IN
{
	float4 pos : SV_POSITION;
	float2 tex : TEXCOORD;
	float3 col : COLOUR;
};

struct GS_OUT
{
	float4 pos : SV_POSITION;
	float4 posWS : WORLD_POSITION;
	float4 norWS : WORLD_NORMAL;
	float2 tex : TEXCOORD;
};

cbuffer GS_CONSTANT_BUFFER : register(b0)
{
	matrix world, worldViewProj;
};

[maxvertexcount(3)]
void GS_main(triangle GS_IN input[3], inout TriangleStream< GS_OUT > output)
{
	GS_OUT element;
	float4 normal = float4(normalize(cross(input[1].pos - input[0].pos, input[2].pos - input[0].pos)), 0);
	for (uint i = 0; i < 3; i++)
	{
		element.pos = mul(input[i].pos, worldViewProj);
		element.posWS = mul(input[i].pos, world);
		element.norWS = mul(normal, world);
		element.tex = input[i].tex;
		output.Append(element);
	}
	output.RestartStrip();
}