/*struct DS_OUTPUT
{
	float4 vPosition : SV_POSITION;
	float4 WorldPosition : WORLDPOS;
	float3 Normal	: NORMAL;
	float2 tex : TEXCOORD0;
};

// Output control point
struct HS_CONTROL_POINT_OUTPUT
{
	float4 vPosition : SV_POSITION;
	float4 WorldPosition : WORLDPOS;
	float3 Normal	: NORMAL;
	float2 tex : TEXCOORD0;
};

// Output patch constant data.
struct HS_CONSTANT_DATA_OUTPUT
{
	float EdgeTessFactor[3]			: SV_TessFactor; // e.g. would be [4] for a quad domain
	float InsideTessFactor			: SV_InsideTessFactor; // e.g. would be Inside[2] for a quad domain
	// TODO: change/add other stuff
};

#define NUM_CONTROL_POINTS 3

[domain("tri")]
DS_OUTPUT main(
	HS_CONSTANT_DATA_OUTPUT input,
	float3 domain : SV_DomainLocation,
	const OutputPatch<HS_CONTROL_POINT_OUTPUT, NUM_CONTROL_POINTS> patch)
{
	DS_OUTPUT Output;

	float3 vPos = patch[0].vPosition*domain.x + patch[1].vPosition*domain.y + patch[2].vPosition*domain.z;
	float3 wPos = patch[0].WorldPosition*domain.x + patch[1].WorldPosition*domain.y + patch[2].WorldPosition*domain.z;
	float3 norm = patch[0].Normal*domain.x + patch[1].Normal*domain.y + patch[2].Normal*domain.z;
	float2 tex = patch[0].tex;

	Output.vPosition = float4(vPos, 1.0);
	Output.WorldPosition = float4(wPos, 1.0);
	Output.Normal = float4(norm, 1.0);
	Output.tex = tex;
	return Output;
}
*/