#include "ConstantBuffer.hlsli"

//--------------------------------------------------------------------------------------
// Globals
//--------------------------------------------------------------------------------------
//// PerObject
cbuffer cbPerObject : register(b2)
{
    Transfrom g_Transform : packoffset(c0);
    Material g_Material : packoffset(c8);
}

//--------------------------------------------------------------------------------------
// Textures and Samplers
//--------------------------------------------------------------------------------------
TextureCube g_SkyBox : register(t0);

SamplerState g_samLinear : register(s0);

//--------------------------------------------------------------------------------------
// Input / Output structures
//--------------------------------------------------------------------------------------
struct PS_INPUT
{
    float4 PosH : SV_POSITION;
    float3 PosL : POSITION;
    float2 UV : TEXCOORD0;
};

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PSMain(PS_INPUT Input) : SV_TARGET
{
    float4 Color;

    Color = g_SkyBox.Sample(g_samLinear, Input.PosL);
    
    // float3 envColor = Color.xyz / (Color.xyz + float3(1.f, 1.f, 1.f));
    
    // envColor = pow(envColor, float3(1.f / 2.2f, 1.f / 2.2f, 1.f / 2.2f));

    return float4(Color.xyz, 1.f);
}