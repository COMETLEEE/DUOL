#include "ConstantBuffer.hlsli"

#define CAMERA_MOTION_BLUR_SAMPLE_COUNT 10

static float g_VelocityScale = 0.2f;

struct VS_OUT
{
    float4 PosH : SV_POSITION;
    float2 Texcoord0 : TEXCOORD0;
};

// 현재 프레임의 위치 버퍼
Texture2D Texture_Position : register(t0);

Texture2D Texture_Target : register(t1);

SamplerState Sam_Clamp : register(s0);

float4 PSMain(VS_OUT input) : SV_TARGET
{
    float4 currentXYZ = Texture_Position.Sample(Sam_Clamp, input.Texcoord0);

    // 엄청 먼 것으로 판정하기.
    if (currentXYZ.a <= 0.000001f)
        return Texture_Target.Sample(Sam_Clamp, input.Texcoord0);

    float4 currentPosition = float4(currentXYZ.xyz, 1.f);
    
    float4 currView = mul(currentPosition, g_Camera.g_ViewMatrix);
    
    float4 currProj = mul(currView, g_Camera.g_ProjectionMatix);
    
    currProj /= currProj.w;
    
    float4 prevView = mul(currentPosition, g_Camera.g_PrevViewMatrix);
    
    float4 prevProj = mul(prevView, g_Camera.g_PrevProjectionMatrix);
    
    prevProj /= prevProj.w;

    float f = 1.0f / CAMERA_MOTION_BLUR_SAMPLE_COUNT;
    
    // '현재 픽셀 -> 이전 픽셀' 벡터 생각
    float2 velocity = float2(prevProj.x - currProj.x, prevProj.y - currProj.y) * g_VelocityScale * f;
    
    float4 color = 0.f;
    
    for (int i = 0; i < CAMERA_MOTION_BLUR_SAMPLE_COUNT; i++)
    {
        float2 newUV = saturate(input.Texcoord0 + velocity.xy * i);
        
        // 속도 방향으로 이동하면서 샘플링 (근데 이거 주변 색상 변화에 따라서 조금 수치를 다르게 하는게 좋을 것 같은데 ..)
        float4 currentColor = Texture_Target.Sample(Sam_Clamp, newUV);
        
        color += currentColor;
    }
    
    return float4((color / CAMERA_MOTION_BLUR_SAMPLE_COUNT).xyz, 1.f);
}