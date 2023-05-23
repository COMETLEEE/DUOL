#include "cbPerFrame.hlsli"

//--------------------------------------------------------------------------------------
// Globals
//--------------------------------------------------------------------------------------
//// PerObject
cbuffer cbPerObject : register(b1)
{

};

//--------------------------------------------------------------------------------------
// Textures and Samplers
//--------------------------------------------------------------------------------------
Texture2D g_DiffuseTexture : register(t0);
Texture2D g_NormalMapTexture : register(t1);

SamplerState g_samLinear : register(s0);

//--------------------------------------------------------------------------------------
// Input / Output structures
//--------------------------------------------------------------------------------------
struct PS_INPUT
{
    float4 PosH : SV_POSITION;
    float2 Texcoord0 : TEXCOORD0;
};

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PSMain(PS_INPUT Input) : SV_TARGET
{
    
    //float4 diffuse = g_DiffuseTexture.Sample(g_samLinear, Input.Texcoord0);
    //
    //clip(diffuse.a - 0.15f);
    
    return float4(1.0f, 1.0f, 1.0f, 1.0f);
}
