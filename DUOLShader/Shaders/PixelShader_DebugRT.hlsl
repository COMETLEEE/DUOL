#include "ConstantBuffer.hlsli"

//--------------------------------------------------------------------------------------
// Globals
//--------------------------------------------------------------------------------------
//// PerObject
cbuffer cbPerObject : register(b2)
{
    int idx;
};

SamplerState g_samLinear : register(s0);

Texture2D gRenderTargetTexture[4];

struct VertexOut
{
    float4 PosH : SV_POSITION;
    float2 Texcoord0 : TEXCOORD0;
};

float4 PSMain(VertexOut pin) : SV_Target
{
    if (idx == 0)
    {
        return gRenderTargetTexture[0].Sample(g_samLinear, pin.Texcoord0);
    }
    else if (idx == 1)
    {
        return gRenderTargetTexture[1].Sample(g_samLinear, pin.Texcoord0);
    }
    else if (idx == 2)
    {
        return gRenderTargetTexture[2].Sample(g_samLinear, pin.Texcoord0);
    }
    else if (idx == 3)
    {
        return gRenderTargetTexture[3].Sample(g_samLinear, pin.Texcoord0);
    }
    
    return gRenderTargetTexture[0].Sample(g_samLinear, pin.Texcoord0);
}
