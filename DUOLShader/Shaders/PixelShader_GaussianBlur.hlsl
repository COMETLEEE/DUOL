
#define BLUR_SAMPLE_COUNT 9

cbuffer cbPerObject : register(b2)
{
	// x = width , y = height, z= 1/width, w = 1/height
	float4 _sceenInfo;
}

static const float g_Weight[BLUR_SAMPLE_COUNT] =
{
		0.013519569015984728f,
		0.047662179108871855f,
		0.11723004402070096f,
		0.20116755999375591f,
		0.240841295721373f,
		0.20116755999375591f,
		0.11723004402070096f,
		0.047662179108871855f,
		0.013519569015984728f
};


static const float g_Indices[BLUR_SAMPLE_COUNT] = { -4,-3,-2,-1,0,+1,+2,+3,+4 };

struct VS_OUT
{
	float4 PosH : SV_POSITION;
	float2 uv : TEXCOORD0;
};

Texture2D g_InputTexture : register(t0);

SamplerState g_samLinear :register(s0);

float4 PSBlurX(VS_OUT input) : SV_TARGET
{
	float2 dir = float2(1.f,0.f);

    uint screenWidth = 0;
    uint screenHeight = 0;
    uint lod = 0;

    g_InputTexture.GetDimensions(0, screenWidth, screenHeight, lod);

    float2 step = float2(dir.x * screenWidth, dir.y * screenHeight);
	float4 color = (float4)0;

	for (int i = 0; i < BLUR_SAMPLE_COUNT; i++)
	{
		float2 step2 = float2(1.f / step.x, 0.f) * g_Indices[i];

		color += g_InputTexture.Sample(g_samLinear, input.uv + step2) * g_Weight[i];
	}

	return color;
}

float4 PSBlurY(VS_OUT input) : SV_TARGET
{
	float2 dir = float2(0.f, 1.f);

    uint screenWidth = 0;
    uint screenHeight = 0;
    uint lod = 0;

    g_InputTexture.GetDimensions(0, screenWidth, screenHeight, lod);

    float2 step = float2(dir.x * screenWidth, dir.y * screenHeight);
	//float2 step = float2(dir.x * _sceenInfo.z, dir.y * _sceenInfo.w);
	float4 color = (float4)0;

	for (int i = 0; i < BLUR_SAMPLE_COUNT; i++)
	{
        float2 step2 = float2(0.f, 1.f / step.y) * g_Indices[i];
        //float2 step2 = float2(0.f, 0.f);
        color += g_InputTexture.Sample(g_samLinear, input.uv + step2) * g_Weight[i];
	}

	return color;
}