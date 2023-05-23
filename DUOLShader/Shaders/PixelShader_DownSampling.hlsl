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

Texture2D gRenderTargetTexture;

struct VertexOut
{
    float4 PosH : SV_POSITION;
    float2 Texcoord0 : TEXCOORD0;
};

float4 PSMain(VertexOut pin) : SV_Target
{
    float4 color = gRenderTargetTexture.Sample(g_samLinear, pin.Texcoord0);

    return color;
}
