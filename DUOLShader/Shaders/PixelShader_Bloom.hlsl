
struct VS_OUT
{
	float4 PosH : SV_POSITION;
	float2 Texcoord0 : TEXCOORD0;
};

Texture2D g_OriginalTexture : register(t0);

Texture2D g_BlurredTexture : register(t1);

SamplerState g_samClamp :register(s0);

float4 PSMain(VS_OUT input) : SV_TARGET
{
	float4 color = (float4)0.f;

    color = g_BlurredTexture.Sample(g_samClamp, input.Texcoord0) + g_OriginalTexture.Sample(g_samClamp, input.Texcoord0);

	return color;
}