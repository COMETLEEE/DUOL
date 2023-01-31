#include "Common.hlsli"
#include "LightHelper.hlsli"
#include "ParticleHelper.hlsli"

#define EPSILON 0.000001f
/**
 * \brief �����Ӹ��� ������Ʈ�� �ؾ��Ѵ�.
 */
cbuffer CB_PerObject : register(b1)
{
	float4x4 gWorldViewProj; // ���� * �� * ��������
	float4x4 gWorld; // ������Ʈ�� ���� ��.
	float4x4 gWorldlnvTranspose; // �븻���� �̵���ų �� ����ġ�� ���ؾ� �Ѵ�.

	float4 ObjectID;
	
    float4 gColor;
};

/**
 * \brief �������ӿ� �ѹ��� ������Ʈ�� �ϸ� �ȴ�.
 */
cbuffer CB_PerFream : register(b0)
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


cbuffer CB_PerFrame_Particle : register(b0)
{
	float3 gCameraPosW; // ī�޶��� ��ǥ
	float pad999;

	float2 gScreenXY;
	float gTimeStep; // 1�����Ӵ� �ð�
	float gGamePlayTime; // ���� ���ۺ��� �帥 �ð�.

	float4x4 gViewProj;
};


SamplerState samAnisotropic : register(s0);
