cbuffer View : register(b0)
{
	matrix View;
	float3 CameraPosition;
}

struct PS_Input
{
	float4 Position : SV_POSITION;
	float4 WorldPosition : WORLDPOS;
	float3 Normal	: NORMAL;
	float2 TextureUV : TEXCOORD;
};

Texture2D Texture : register(t0);
SamplerState Sampler : register(s0);

float4 main(PS_Input Input) : SV_TARGET
{
	float4 Ambient = { 0.2f, 0.2f, 0.2f, 1.0f };
	float3 SunColour = { 1.0f, 1.0f, 1.0f };
	float3 SunDirection = { 20.0f, 20.0f, 20.0f };

	float3 CameraDirection = CameraPosition - Input.WorldPosition;
	float CameraDistance = length(CameraDirection);
	CameraDirection = normalize(CameraDirection);

	float4 Atmosphere = {0.46f, 0.76f, 0.88f, 1.0f};

	float FogStart = 100.0f;
	float FogFalloff = 0.1f;

	float T = CameraDistance - FogStart;
	if (T < 0) T = 0;
	T = T * FogFalloff * 0.0035f;

	float3 Stone = {0.73f, 0.71f, 0.67f };
	float3 Grass = { 0.31f, 0.56f, 0.2f };
	float3 Snow = { 1.0f, 1.0f, 1.0f };
	float3 Up = { 0.0f, 1.0f, 0.0f };
	float3 Diffuse = Grass;

	float AU = saturate(dot(Input.Normal, Up));

	if (Input.WorldPosition.y > 400)
	{
		Diffuse = Snow;
	}

	if (Input.WorldPosition.y <= 400 && Input.WorldPosition.y >= 350)
	{
		float I = Input.WorldPosition.y - 350.0f;
		I = I / 50.0f;
		Diffuse = Diffuse + (Snow - Diffuse) * I;
	}
		
	if (AU < 0.67f)
	{
		Diffuse = Stone;
	}

	if (AU <= 0.72f && AU >= 0.67f)
	{
		float I = AU - 0.67f;

		I = I / 0.05f;

		Diffuse = Stone + (Diffuse - Stone) * I;
	}

	SunDirection = normalize(SunDirection);

	float NDotL = dot(SunDirection, Input.Normal);
	float3 Final = SunColour * saturate(NDotL);

	Diffuse = Texture.Sample(Sampler, Input.TextureUV);

	float4 Lit = (saturate(Ambient + float4(Final, 1.0f)) * float4(Diffuse, 1.0f));
	Lit = Lit + (Atmosphere - Lit) * T;

	//Lit = float4(Diffuse, 1.0f);

	return Lit;
}