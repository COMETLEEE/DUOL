#include "ConstantBuffer.hlsli"

//--------------------------------------------------------------------------------------
// Globals
//--------------------------------------------------------------------------------------
//// PerObject
cbuffer cbPerObject : register(b2)
{
    matrix skyboxMatrix;
}

//--------------------------------------------------------------------------------------
// Input / Output structures
//--------------------------------------------------------------------------------------
struct VS_INPUT
{
    float3 Position : POSITION;
    float2 Texcoord0 : TEXCOORD0;
    float3 Normal : NORMAL;
    float3 Tangent : TANGENT;
};

struct VS_OUTPUT
{
    float4 PosH : SV_POSITION;
    float3 PosL : POSITION;
    float2 UV : TEXCOORD0;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VSMain(VS_INPUT Input)
{
    VS_OUTPUT vout;

    float4 worldPos = mul(float4(Input.Position, 1.f), skyboxMatrix);
    float4 ret = mul(worldPos, g_Camera.g_ViewProjectionMatrix);
    vout.PosH = ret.xyww;
    
    vout.PosL = Input.Position;
    vout.UV = Input.Texcoord0;
    
    return vout;
}