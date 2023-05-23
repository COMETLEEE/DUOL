#include "ConstantBuffer.hlsli"

SamplerState g_samLinear : register(s0);

Texture2D g_Albedo : register(t0);
Texture2D g_Normal : register(t1);
Texture2D g_PosW : register(t2);
Texture2D g_MetallicRoughnessAOSpecular : register(t3);

//--------------------------------------------------------------------------------------
// Input / Output structures
//--------------------------------------------------------------------------------------
struct VS_OUTPUT
{
    float4 PosH : SV_POSITION;
    float2 Texcoord0 : TEXCOORD0;
};

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PSMain(VS_OUTPUT Input) : SV_TARGET
{
    float4 ret = g_Albedo.Sample(g_samLinear, Input.Texcoord0);

    if ((ret.x == 0.f) && (ret.y == 0.f) && (ret.z == 0.f))
        clip(-1);

    return g_Albedo.Sample(g_samLinear, Input.Texcoord0);
}