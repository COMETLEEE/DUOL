#include "ConstantBuffer.hlsli"

//--------------------------------------------------------------------------------------
// Globals
//--------------------------------------------------------------------------------------
#define SSAO_KERNEL_SIZE 64
#define radius 0.5f
#define bias 0.25f
// PerObject
cbuffer cbPerObject : register(b2)
{
    float3 SSAO_KERNEL[SSAO_KERNEL_SIZE];
};

//--------------------------------------------------------------------------------------
// Textures and Samplers
//--------------------------------------------------------------------------------------

Texture2D g_PrevSSAO : register(t0);

SamplerState g_samLinear : register(s0);
SamplerState g_samMirror : register(s1);

//--------------------------------------------------------------------------------------
// Input / Output structures
//--------------------------------------------------------------------------------------

struct PS_IN
{
    float4 PosH : SV_POSITION;
    float2 Texcoord0 : TEXCOORD0;
};

struct PS_OUT
{
    float SSAO;
};

//--------------------------------------------------------------------------------------
// Functions
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float PSMain(PS_IN Input) : SV_TARGET
{
    float2 texelSize = 1.0 / float2(g_ScreenXY);
    float result = 0.0;
    for (int x = -2; x < 2; ++x)
    {
        for (int y = -2; y < 2; ++y)
        {
            float2 offset = float2(float(x), float(y)) * texelSize;
            result += g_PrevSSAO.Sample(g_samLinear, Input.Texcoord0 + offset).r;
        }
    }
    
    float output = result / (4.0 * 4.0);

    return output;
}