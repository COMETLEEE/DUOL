#include "LightHelper.hlsli"

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
    float3 gEmitPosW; // ����� ��ǥ
    float gStartSpeed; // ��ƼŬ ��� �ӵ�.

    float3 gEmitDirW; // ����� ����
    float gLifeTime; // ��ƼŬ�� �󸶳� ���������?

    float2 gStartSize;
    uint gEmissiveCount; // �ѹ��� �����ϴ� ��ƼŬ�� ��.
    float gEmissiveTime; // ���� �ֱ�

    float3 gAccelW = { 0.0f, 7.8f, 0.0f }; // ���ӵ�
    float pad1;
};

cbuffer CB_PerFrame_Particle : register(b1)
{
    float3 gCameraPosW; // ī�޶��� ��ǥ
    float gGameTime; // ���� �ð�

    float2 gScreenXY;
    float gTimeStep; // 1�����Ӵ� �ð�
    float pad2;

    float4x4 gViewProj;
};




SamplerState samAnisotropic : register(s0);
