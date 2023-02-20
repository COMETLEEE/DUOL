#include "Common.hlsli"
#include "LightHelper.hlsli"
#include "ParticleHelper.hlsli"

#define EPSILON 0.000001f


struct DeferredPSOut
{
    float4 Albedo : SV_Target0;
    float4 Depth : SV_Target1;
    float4 Normal : SV_Target2;
    float4 Position : SV_Target3;
    float4 MetalicRoughnessAosPecular : SV_Target4;
    float4 ObejctID : SV_Target5;
};

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
cbuffer CB_PerFrame : register(b0)
{
    Light gLight[MAX_LIGHT_CNT];
	
    float3 gCameraPosW; // ī�޶��� ��ǥ
    uint gLightCount;

    float2 gScreenXY;
    float gTimeStep; // 1�����Ӵ� �ð�
    float gGamePlayTime; // ���� ���ۺ��� �帥 �ð�.

    float4x4 gViewProj;
};




SamplerState samAnisotropic : register(s0);
