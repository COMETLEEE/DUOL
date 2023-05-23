#include "ConstantBuffer.hlsli"

//--------------------------------------------------------------------------------------
// Globals
//--------------------------------------------------------------------------------------
//// PerObject
cbuffer cbPerObject : register(b2)
{
};

//--------------------------------------------------------------------------------------
// Input / Output structures
//--------------------------------------------------------------------------------------
struct VS_INPUT
{
    float3 Position : POSITION;
    float2 Texcoord0 : TEXCOORD0;
};

struct VS_OUTPUT
{
    float4 PosH : SV_POSITION;
    float3 ViewRay : VIEWRAY;
    float2 Texcoord0 : TEXCOORD;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VSMain(VS_INPUT Input)
{
    VS_OUTPUT Output;

    Output.PosH = float4(Input.Position, 1.f);
    float4 PosView = mul(float4(Input.Position.xy, 0, 1.f), g_Camera.g_InverseProjectionMatrix); ;
    Output.ViewRay = normalize(PosView.xyz / PosView.w);
    Output.Texcoord0 = Input.Texcoord0;

    return Output;
}