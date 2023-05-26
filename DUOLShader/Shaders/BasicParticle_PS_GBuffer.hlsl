#include "ConstantBuffer.hlsli"
#include "ParticleHelper.hlsli"

Texture2DArray gTexArray : register(t0);

//Texture2D gDepthBuffer : register(t1); // 파티클을 그릴 때 앞에 오브젝트가 있으면 그리지 않기 위해서 뎁스버퍼를 참조한다.
// 기존의 뎁스 값, 불투명한 오브젝트를 그린 뎁스값이다.
//Texture2D gPreDepthBuffer : register(t2); // 파티클을 그릴 때 앞에 오브젝트가 있으면 그리지 않기 위해서 뎁스버퍼를 참조한다.
// OIT Layer를 그릴 때 생긴 뎁스 값이다.

SamplerState samAnisotropic : register(s0);

struct DepthOut
{
    float4 Normal : SV_Target0;
    float4 World : SV_Target1;
    float4 MetalicRoughnessAOSpecular : SV_Target2; // 4byte pack 256 256 256 256
    float4 ObjectID : SV_Target3;                   // 8byte
    float4 Effect : SV_Target4;
};

struct GeoOut
{
    float4 PosH : SV_Position;
    float4 Color : COLOR;
    float2 Tex : TEXCOORD1;
};

[earlydepthstencil]
DepthOut OIT_Particle_PS(GeoOut pin) // 픽셀을 저장하는 pixel shader
{
    DepthOut depthOut;

    depthOut.Normal = float4(0.f, 1.f, 0.f, 0.f);
    depthOut.World = float4(pin.PosH);
    depthOut.MetalicRoughnessAOSpecular = float4(0.f, 1.f, 0.f, 0.f);
    depthOut.ObjectID = float4(1.f, 1.f, 1.f, 1.f);
    depthOut.Effect = float4(0.f, 0.f, 0.f, 0.f);

    return depthOut;
}