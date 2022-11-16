#include "LightHelper.hlsli"

/**
 * \brief 프레임마다 업데이트를 해야한다.
 */
cbuffer CB_PerObject : register(b0)
{
    float4x4 gWorldViewProj; // 월드 * 뷰 * 프로젝션
    float4x4 gWorld; // 오브젝트의 월드 값.
    float4x4 gWorldlnvTranspose; // 노말값을 이동시킬 때 역전치를 곱해야 한다.
};

/**
 * \brief 한프레임에 한번만 업데이트를 하면 된다.
 */
cbuffer CB_PerFream : register(b1)
{
    DirectionalLight gDirLight;
    PointLight gPointLight[10];
    SpotLight gSpotLight;
    float3 gEyePosW;
    int gPointCount;
    float4x4 gLightViewProj;
    
    float4x4 gCurrentViewProj; // 블러를 위한 것! 엥 필요없었네?
    float4x4 gPrevViewProj; // 블러를 위한 것!
}



cbuffer CB_PerObject_Particle : register(b0)
{
    float3 gEmitPosW; // 방출기 좌표
    float gStartSpeed; // 파티클 출발 속도.

    float3 gEmitDirW; // 방출기 방향
    float gLifeTime; // 파티클이 얼마나 살아있을까?

    float2 gStartSize;
    uint gEmissiveCount; // 한번에 방출하는 파티클의 수.
    float gEmissiveTime; // 방출 주기

    float3 gAccelW = { 0.0f, 7.8f, 0.0f }; // 가속도
    float pad1;
};

cbuffer CB_PerFrame_Particle : register(b1)
{
    float3 gCameraPosW; // 카메라의 좌표
    float gGameTime; // 게임 시간

    float2 gScreenXY;
    float gTimeStep; // 1프레임당 시간
    float pad2;

    float4x4 gViewProj;
};




SamplerState samAnisotropic : register(s0);
