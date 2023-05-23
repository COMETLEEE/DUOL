#include "ConstantBuffer.hlsli"

//--------------------------------------------------------------------------------------
// Globals
//--------------------------------------------------------------------------------------
//// PerObject

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
struct PS_INPUT
{
    float4 PosH : SV_POSITION;
    float3 PosW : POSITION;
    float3 Tangent : TANGENT;
    float3 Normal : NORMAL;
    float2 Texcoord0 : TEXCOORD0;
};

struct PS_OUT
{
    float4 SceneView : SV_Target0;
    float4 ObjectID : SV_Target1;
};

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
PS_OUT PSMain(PS_INPUT Input) : SV_Target
{
    PS_OUT psOut;

    psOut.SceneView = g_Material.Albedo;

    psOut.ObjectID = float4(g_ObjectID, 0.f, 0.f);

    return psOut;
}