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


VertexOut vs_main(Vertexln vin)
{
    VertexOut vout;

    vout.PosW = mul(float4(vin.PosL, 1.0f), gWorld).xyz;
    vout.PosH = mul(float4(vin.PosL, 1.0f), gWorldViewProj);
    vout.PosL = vin.PosL;

    return vout;
}



DeferredPSOut ps_main(VertexOut pin) : SV_TARGET
{

    DeferredPSOut _psOut;

    _psOut.Position = float4(0.0f, 0.0f, 0.0f, 0.0f);
    _psOut.Depth = float4(0.0f, 0.0f, 0.0f, 0.0f);
    _psOut.Albedo = gCubeMap.Sample(samAnisotropic, pin.PosL) * gColor;
    _psOut.Normal = float4(0.0f, 0.0f, 0.0f, 0.0f);
    _psOut.MetalicRoughnessAoSpecular = float4(0.0f, 0.0f, 0.0f, 0.0f);
    _psOut.ObejctID = float4(0.0f, 0.0f, 0.0f, 0.0f);

    return _psOut;
}