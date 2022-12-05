#include "ConstantBuffer.hlsli"

struct Vertexln
{
    float3 PosL : POSITION;
    float2 Tex : TEXCOORD;
    float3 NormalL : NORMAL;
    float4 Color : COLOR;
    float3 BoneWeight : WEIGHT;
    uint4 BoneIndex : BONEINDEX;
    float3 tangent : TANGENT;
};

struct VertexOut
{
    float4 PosH : SV_POSITION;
    float3 PosW : POSITION;
    float3 NormalW : NORMAL;
    float4 Color : COLOR;
    float2 Tex : TEXCOORD;
    float3 tangent : TANGENT;
};

VertexOut main(Vertexln vin)
{
    VertexOut vout;

    vout.PosW = mul(float4(vin.PosL, 1.0f), gWorld).xyz;
    vout.NormalW = mul(vin.NormalL, (float3x3) gWorldlnvTranspose); //¿ªÀüÄ¡
    vout.PosH = mul(float4(vin.PosL, 1.0f), gWorldViewProj);
    vout.Color = vin.Color;
    vout.Tex = vin.Tex;
    vout.tangent = vin.tangent;

    return vout;
}


