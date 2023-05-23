#include "ConstantBuffer.hlsli"

cbuffer cbPerObject : register(b2)
{
    float2 g_ObjectID;
    
    float2 g_pad1;
    
    Transfrom g_Transform; //: packoffset(c0);
    Material g_Material; //: packoffset(c8);
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
    float3 PosW : POSITION;
    float3 Tangent : TANGENT;
    float3 Normal : NORMAL;
    float2 Texcoord0 : TEXCOORD0;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VSMain(VS_INPUT Input)
{
    VS_OUTPUT Output;

    matrix wvp = mul(g_Transform.g_World, g_Camera.g_ViewProjectionMatrix);
    
    Output.PosH = mul(float4(Input.Position, 1.0f), wvp);
    Output.PosW = mul(float4(Input.Position, 1.0f), g_Transform.g_World).xyz;
    Output.Tangent = normalize(mul(Input.Tangent, (float3x3) g_Transform.g_WorldInvTranspose));
    Output.Normal = normalize(mul(Input.Normal, (float3x3) g_Transform.g_WorldInvTranspose));

    Output.Texcoord0 = Input.Texcoord0;
    
    return Output;
}