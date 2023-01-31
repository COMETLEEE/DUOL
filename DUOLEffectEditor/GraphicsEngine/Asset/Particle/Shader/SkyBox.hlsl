#include "ConstantBuffer.hlsli"

TextureCube gCubeMap : register(t0);

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
    float3 PosW : POSITION0;
    float3 PosL : POSITION1;
};
struct PSOut
{
    float4 Depth : SV_Target0;
    float4 Normal : SV_Target1;
    float4 Position : SV_Target2;
    float4 Albedo : SV_Target3;
    float4 Diffuse : SV_Target4;
    float4 Specular : SV_Target5;
    float4 ObejctID : SV_Target6;
};


VertexOut vs_main(Vertexln vin)
{
    VertexOut vout;

    vout.PosW = mul(float4(vin.PosL, 1.0f), gWorld).xyz;
    vout.PosH = mul(float4(vin.PosL, 1.0f), gWorldViewProj);
    vout.PosL = vin.PosL;

    return vout;
}



PSOut ps_main(VertexOut pin) : SV_TARGET
{

    PSOut _psOut;

    _psOut.Position = float4(0.0f, 0.0f, 0.0f, 0.0f);
    _psOut.Depth = float4(0.0f, 0.0f, 0.0f, 0.0f);
    _psOut.Albedo = gCubeMap.Sample(samAnisotropic, pin.PosL) * gColor;
    _psOut.Normal = float4(0.0f, 0.0f, 0.0f, 0.0f);
    _psOut.Diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    _psOut.Specular = float4(0.0f, 0.0f, 0.0f, 0.0f);
    _psOut.ObejctID = float4(0.0f, 0.0f, 0.0f, 0.0f);

    return _psOut;
}