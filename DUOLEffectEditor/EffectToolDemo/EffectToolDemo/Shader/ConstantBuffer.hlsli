#include "LightHelper.hlsli"

/**
 * \brief �����Ӹ��� ������Ʈ�� �ؾ��Ѵ�.
 */
cbuffer CB_PerObject : register(b0)
{
    float4x4 gWorldViewProj;
};

/**
 * \brief �������ӿ� �ѹ��� ������Ʈ�� �ϸ� �ȴ�.
 */
cbuffer CB_PerFream : register(b1)
{
    DirectionalLight gDirLight;
    PointLight gPointLight[10];
    SpotLight gSpotLight;
    float3 gEyePosW;
    int gPointCount;
    float4x4 gLightViewProj;
    
    float4x4 gCurrentViewProj; // ���� ���� ��! �� �ʿ������?
    float4x4 gPrevViewProj; // ���� ���� ��!
}