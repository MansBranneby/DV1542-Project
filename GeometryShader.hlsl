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
void GS_main( triangle GS_IN input[3], inout TriangleStream< GS_OUT > output)
{
	GS_OUT element;
	float4 normal = float4(normalize(cross(input[1].pos.xyz - input[0].pos.xyz, input[2].pos.xyz - input[0].pos.xyz)), 0);
	
	//Backface culling
	float4 position0VP = mul(input[0].pos, worldViewProj);
	float4 position1VP = mul(input[1].pos, worldViewProj);
	float4 position2VP = mul(input[2].pos, worldViewProj);

	float4 normalVP = normalize(float4(cross(position1VP.xyz - position0VP.xyz, position2VP.xyz - position0VP.xyz), 0.0f));
	bool frontFace = false;
	if (dot(position0VP, normalVP) < 0)
		frontFace = true;
	////

	for (uint i = 0; i < 3; i++)
	{
		if (frontFace)
			element.pos = mul(input[i].pos, worldViewProj);

		element.posWS = mul(input[i].pos, world);
		element.norWS = mul(normal, world);
		element.tex = input[i].tex;
		output.Append(element);
	}
	output.RestartStrip();
}