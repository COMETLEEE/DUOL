#include "LightHelper.hlsli"

/**
 * \brief 프레임마다 업데이트를 해야한다.
 */
cbuffer CB_PerObject : register(b0)
{
    float4x4 gWorldViewProj;
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