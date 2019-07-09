struct VS_IN
{
	float3 position : POSITION;
};

struct HS_IN
{
	float3 position : POSITION;
};

HS_IN main(VS_IN Input)
{
	HS_IN Output;

	Output.position = Input.position;

	return Output;
}