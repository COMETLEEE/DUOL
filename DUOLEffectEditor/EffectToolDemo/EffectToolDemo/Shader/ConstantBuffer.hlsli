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
    float3 gEmitDirW;
    float gDuration; // 몇 초 동안 파티클 객체가 재생될 지.					

    float3 gEmitPosW;
    int gMaxParticles; // 파티클 최대 출력 사이즈.							

    float2   gStartDelay; // 몇 초 뒤에 파티클이 재생될 지.					
    float2   gStartLifeTime; // 한 파티클의 생존 시간.						

    float2   gStartSpeed; // 파티클 생성시 시작 속도.
    float2 gStartRotation; // 파티클의 시작 회전.							

    float4  gStartSize; // 파티클의 시작 크기.							

    float4  gStartColor[2]; // 파티클의 시작 색상									

    float2   gGravityModifier; // 파티클에 가해지는 중력.
    float gParticlePlayTime;
    float   pad4; // 파티클에 가해지는 중력.						

    int gisLooping; // 반복여부.
    int gVertexCount; // 파티클 갯수 확인..
    int2 pad3; // 시작인가요 ..?						

		////// 임시 변수

    float gEmissiveCount; // 한번에 몇개를 방출 시킬지.

    float gEmissiveTime; // 다음 방출까지 걸리는 시간.

    float2 pad5;
};



cbuffer CB_PerFrame_Particle : register(b1)
{
    float3 gCameraPosW; // 카메라의 좌표
    float pad999;

    float2 gScreenXY;
    float gTimeStep; // 1프레임당 시간
    float pad2;

    float4x4 gViewProj;
};


SamplerState samAnisotropic : register(s0);
