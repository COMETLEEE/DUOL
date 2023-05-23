#include "ConstantBuffer.hlsli"

//--------------------------------------------------------------------------------------
// Globals
//--------------------------------------------------------------------------------------
//// PerObject
cbuffer cbPerObject : register(b2)
{
    int idx;
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
    float2 Texcoord0 : TEXCOORD0;
};


//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VSMain(VS_INPUT Input)
{
    VS_OUTPUT Output;
    
    const float height = 0.3f;
    const float width = 0.4f;
    
    Output.PosH = saturate(float4(Input.Position, 1.f));
    
    [flatten]
    if (Output.PosH.x <= 0.1f)
    {
        Output.PosH.x = -1 + idx * width;
    }
    else
    {
        Output.PosH.x = -1 + (idx + 1) * width;
    }
    
    if (Output.PosH.y <= 0.1f)
    {
        Output.PosH.y = 1 - height;
    }
    
    
    Output.Texcoord0 = Input.Texcoord0;
    
    return Output;
}