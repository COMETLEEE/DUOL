#include "LightHelper.hlsli"
#include "ParticleHelper.hlsli"

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
    CommonInfo gCommonInfo;
    
    Emission gEmission;
    
    Velocity_Over_LifeTime gVelocityOverLifetime;
    
    Color_Over_LifeTime gColorOverLifetime;

    Size_Over_Lifetime gSizeOverLifetime;

    Rotation_Over_Lifetime gRotationOverLifetime;

    Texture_Sheet_Animation gTextureSheetAnimation;
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
