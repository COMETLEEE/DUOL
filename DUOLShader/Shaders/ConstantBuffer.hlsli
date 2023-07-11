#include "Common.hlsli"
#include "LightHelper.hlsli"

struct Material  //1 + 1 + 1
{
    float4 Albedo;

    float3 Emissive;
    float EmissivePower;

    float Roughness;
    float Specular;
    float Metalic;
    float pad2;

    float2 Tiling;
    float2 Offset;
};
 
struct Camera // 4 * 7 (28) + 4 = 32
{
    matrix g_ViewMatrix;
    matrix g_ProjectionMatix;
    matrix g_PrevViewMatrix;
    matrix g_PrevProjectionMatrix;
    
    matrix g_ViewProjectionMatrix;
    matrix g_ViewProjectionInverseTransposeMatrix;
    matrix g_InverseProjectionMatrix;
    
    float3 g_CameraPosition;
    float g_CameraVerticalFOV; // vertical

    float3 g_CameraLook;
    float g_CameraFar;

    float3 g_CameraUp;
    float g_CameraNear;

    float g_AspectRatio;
    float3 pad1;
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
	float g_GamePlayTime : packoffset(c0.x); // ���� ���ۺ��� �帥 �ð�.

    uint2 g_ScreenXY : packoffset(c0.y);
    
    float g_TimeStep : packoffset(c0.w); // 1�����Ӵ� �ð�
};

cbuffer cbPerCamera : register(b1)
{
    Camera g_Camera : packoffset(c0);
    
    CascadeShadow g_shadow : packoffset(c32);
       
    Light g_Light[MAX_LIGHT_CNT] : packoffset(c50);

    int g_LightCnt : packoffset(c950.x);
};

float4 UnpackingColor(uint packedVal)
{
    float4 val;

	val.x = packedVal >> 24;
	val.y = (float)((packedVal >> 16) & 0xff);
	val.z = (float)((packedVal >> 8) & 0xff);
    val.w = (float)((packedVal) & 0xff);

    val.xyzw /= 255;

	return val;
};

uint PackingColor(float4 unpackedUnorm)
{
    uint ret = 0;

    uint4 color = unpackedUnorm * 255;
    
	ret =  color.x << 24 ;
	ret += color.y << 16;
	ret += color.z << 8;
	ret += color.w << 0;

	return ret;
};

struct ClosestTransparencyStruct 
{
    float depth;
    float3 world;
    uint MRAS;
    float3 normal;
    float4 ObjectID;
    float4 Effect;
};