struct HS_IN
{
	float3 position : POSITION;
};

struct CONST_OUT
{
	float edges[3] : SV_TessFactor;
	float inside : SV_InsideTessFactor;
};

struct HS_OUT
{
	float3 position : POSITION;
};

CONST_OUT ConstFunc(InputPatch<HS_IN, 3> inputPatch, uint patchId : SV_PrimitiveID)
{
	CONST_OUT Output;

	Output.edges[0] = 15.0f;
	Output.edges[1] = 15.0f;
	Output.edges[2] = 15.0f;

	Output.inside = 15.0f;

	return Output;
}

[domain("tri")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("ConstFunc")]
HS_OUT main(InputPatch<HS_IN, 3> patch, uint pointId : SV_OutputControlPointID, uint patchID : SV_PrimitiveID)
{
	HS_OUT Output;

	Output.position = patch[pointId].position;
	return Output;
}
