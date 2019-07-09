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

struct VS_INPUT
{
	float4 vPos		: POSITION;
	float4 vNormal	: NORMAL;
	float4 vTex		: TEXCOORD;
};

struct PS_INPUT
{
	float4 Position			: SV_POSITION;
	float4 WorldPosition	: WORLDPOS;
	float3 Normal			: NORMAL;
	float2 tex				: TEXCOORD;
};

PS_INPUT main(VS_INPUT Input)
{
	PS_INPUT Output;

	float4 pos = Input.vPos;

	pos = mul(pos, World);
	Output.WorldPosition = pos;
	pos = mul(pos, View);
	pos = mul(pos, Projection);

	Output.Position = pos;
	Output.Normal = Input.vNormal.xyz;
	Output.Normal = mul(Input.vNormal, (float3x3)World);
	Output.Normal = normalize(Output.Normal);

	Output.tex = Input.vTex;

	return Output;
}