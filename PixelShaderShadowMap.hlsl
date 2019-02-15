struct VS_OUT
{
	float4 pos : SV_POSITION;
};

float PS_main(VS_OUT input) :SV_Target
{
	return input.pos.z;
};