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
    float3 gEmitDirW;
    float gDuration; // �� �� ���� ��ƼŬ ��ü�� ����� ��.					

    float3 gEmitPosW;
    int gMaxParticles; // ��ƼŬ �ִ� ��� ������.							

    float2   gStartDelay; // �� �� �ڿ� ��ƼŬ�� ����� ��.					
    float2   gStartLifeTime; // �� ��ƼŬ�� ���� �ð�.						

    float2   gStartSpeed; // ��ƼŬ ������ ���� �ӵ�.
    float2 gStartRotation; // ��ƼŬ�� ���� ȸ��.							

    float4  gStartSize; // ��ƼŬ�� ���� ũ��.							

    float4  gStartColor[2]; // ��ƼŬ�� ���� ����									

    float2   gGravityModifier; // ��ƼŬ�� �������� �߷�.
    float gParticlePlayTime;
    float   pad4; // ��ƼŬ�� �������� �߷�.						

    int gisLooping; // �ݺ�����.
    int gVertexCount; // ��ƼŬ ���� Ȯ��..
    int2 pad3; // �����ΰ��� ..?						

		////// �ӽ� ����

    float gEmissiveCount; // �ѹ��� ��� ���� ��ų��.

    float gEmissiveTime; // ���� ������� �ɸ��� �ð�.

    float2 pad5;
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
