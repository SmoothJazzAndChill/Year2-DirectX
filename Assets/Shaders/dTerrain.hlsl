cbuffer Projection : register(b0)
{
	matrix Projection;
}

cbuffer View : register(b1)
{
	matrix View;
	vector CameraPosition;
}

cbuffer World : register(b2)
{
	matrix World;
}

struct CONST_OUT
{
	float edges[3] : SV_TessFactor;
	float inside : SV_InsideTessFactor;
};

struct HS_OUT
{
	float3 position : POSITION;
};

struct PS_IN
{
	float4 position : SV_POSITION;
};

[domain("tri")]
PS_IN main(CONST_OUT input, float3 uvwCoord : SV_DomainLocation, const OutputPatch<HS_OUT, 3> patch)
{
	float3 vPosition;
	PS_IN Output;

	vPosition = uvwCoord.x * patch[0].position + uvwCoord.y * patch[1].position + uvwCoord.z * patch[2].position;

	float4 pos = float4(vPosition, 1.0f);

	pos = mul(pos, World);
	pos = mul(pos, View);
	pos = mul(pos, Projection);

	Output.position = pos;

	return Output;
}