struct GS_IN
{
	float4 pos : SV_POSITION;
	float2 tex : TEXCOORD;
	float3 col : COLOUR;
};

struct GS_OUT
{
	float4 pos : SV_POSITION;
	float4 worldPos : World_POSITION;
	float4 worldNor : World_NORMAL;
	float2 tex : TEXCOORD;
	float3 col : COLOUR;
};

cbuffer GS_CONSTANT_BUFFER : register(b0)
{
	matrix world, worldViewProj;
};

cbuffer GS_CONSTANT_BUFFER : register(b1)
{
	float3 camPos;
};

cbuffer GS_CONSTANT_BUFFER : register(b2)
{
	float billboardHeight, billboardWidth;
};

[maxvertexcount(4)]
void GS_main(point GS_IN input[1] : SV_POSITION, inout TriangleStream< GS_OUT > output)
{
	float halfHeight = billboardHeight / 2.0f;
	float halfWidth = billboardWidth / 2.0f;

	float3 billboardNormal = camPos.xyz - input[0].pos.xyz;
	billboardNormal = normalize(billboardNormal);

	float3 WorldUpVector = { 0.0f, 1.0f, 0.0f };
	float3 billboardRightVector = normalize(cross(billboardNormal, WorldUpVector));
	float3 billboardUpVector = normalize(cross(billboardRightVector, billboardNormal));
	billboardRightVector *= halfWidth;
	billboardUpVector *= halfHeight;

	float3 billboardVertices[4];
	billboardVertices[0] = input[0].pos.xyz - billboardRightVector - billboardUpVector;
	billboardVertices[1] = input[0].pos.xyz - billboardRightVector + billboardUpVector;
	billboardVertices[2] = input[0].pos.xyz + billboardRightVector - billboardUpVector;
	billboardVertices[3] = input[0].pos.xyz + billboardRightVector + billboardUpVector;

	GS_OUT element;
	for (int i = 0; i < 4; i++)
	{
		element.pos = mul(float4(billboardVertices[i].xyz, 1.0f), worldViewProj);
		element.worldPos = mul(billboardVertices[i], world);

		element.worldNor = float4( 0.0f, 0.0f, 0.0f, 0.0f);
		element.tex = input[0].tex;
		element.col = float4(1.0f, 1.0f, 1.0f, 1.0f);
		output.Append(element);
	}
}