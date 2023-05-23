#include "LightHelper.hlsli"

struct Material
{
    float4 Albedo;
    float3 Emissive;
    float Metalic;
    float Roughness;
    float Specular;
    float2 Tiling;
};

struct Camera // 4 * 4 (16) + 1 + 1 = 18
{
    matrix g_ViewMatrix;
    matrix g_ProjectionMatix;
    matrix g_ViewProjectionMatrix;
    matrix g_ViewProjectionInverseTransposeMatrix;
    
    float3 g_CameraPosition;
    float g_pad;

    float g_CameraFar;
    float g_CameraNear;
    float g_CameraVerticalFOV; // vertical
    float g_AspectRatio;
};

struct Transfrom
{
    matrix g_World;
    matrix g_WorldInvTranspose;
};

//--------------------------------------------------------------------------------------
// Globals
//--------------------------------------------------------------------------------------
//// PerFrame
cbuffer cbPerFrame : register(b0)
{
    Camera g_Camera : packoffset(c0);
    
    uint g_LightCnt : packoffset(c18.x);
    
	float g_GamePlayTime : packoffset(c19.x); // 게임 시작부터 흐른 시간.

    uint2 g_ScreenXY : packoffset(c19.y);
    
    float g_TimeStep : packoffset(c19.w); // 1프레임당 시간
   
    Light g_Light[MAX_LIGHT_CNT] : packoffset(c20);

    CascadeShadow g_shadow : packoffset(c260);
};
