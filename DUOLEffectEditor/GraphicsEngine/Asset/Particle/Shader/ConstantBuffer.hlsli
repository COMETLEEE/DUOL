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
    float4 MetalicRoughnessAoSpecular : SV_Target4;
    float4 ObejctID : SV_Target5;
};

/**
 * \brief 프레임마다 업데이트를 해야한다.
 */
cbuffer CB_PerObject : register(b1)
{
    float4x4 gWorldViewProj; // 월드 * 뷰 * 프로젝션
    float4x4 gWorld; // 오브젝트의 월드 값.
    float4x4 gWorldlnvTranspose; // 노말값을 이동시킬 때 역전치를 곱해야 한다.

    float4 ObjectID;
	
    float4 gColor;
    
    float4 gMetalicRoughnessAoSpecular;
};

/**
 * \brief 한프레임에 한번만 업데이트를 하면 된다.
 */
cbuffer CB_PerFrame : register(b0)
{
    Light gLight[MAX_LIGHT_CNT];
	
    float3 gCameraPosW; // 카메라의 좌표
    uint gLightCount;

    float2 gScreenXY;
    float gTimeStep; // 1프레임당 시간
    float gGamePlayTime; // 게임 시작부터 흐른 시간.

    float4x4 gViewProj;
};




SamplerState samAnisotropic : register(s0);
