#include "LightHelper.hlsli"
#include "ParticleHelper.hlsli"

/**
 * \brief �����Ӹ��� ������Ʈ�� �ؾ��Ѵ�.
 */
cbuffer CB_PerObject : register(b0)
{
    float4x4 gWorldViewProj; // ���� * �� * ��������
    float4x4 gWorld; // ������Ʈ�� ���� ��.
    float4x4 gWorldlnvTranspose; // �븻���� �̵���ų �� ����ġ�� ���ؾ� �Ѵ�.
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
    float3 gCameraPosW; // ī�޶��� ��ǥ
    float pad999;

    float2 gScreenXY;
    float gTimeStep; // 1�����Ӵ� �ð�
    float pad2;

    float4x4 gViewProj;
};


SamplerState samAnisotropic : register(s0);
