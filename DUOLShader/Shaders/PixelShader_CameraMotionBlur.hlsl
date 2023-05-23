#include "ConstantBuffer.hlsli"

#define CAMERA_MOTION_BLUR_SAMPLE_COUNT 10

static float g_VelocityScale = 0.2f;

struct VS_OUT
{
    float4 PosH : SV_POSITION;
    float2 Texcoord0 : TEXCOORD0;
};

// ���� �������� ��ġ ����
Texture2D Texture_Position : register(t0);

Texture2D Texture_Target : register(t1);

SamplerState Sam_Clamp : register(s0);

float4 PSMain(VS_OUT input) : SV_TARGET
{
    float4 currentXYZ = Texture_Position.Sample(Sam_Clamp, input.Texcoord0);

    // ��û �� ������ �����ϱ�.
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
    
    // '���� �ȼ� -> ���� �ȼ�' ���� ����
    float2 velocity = float2(prevProj.x - currProj.x, prevProj.y - currProj.y) * g_VelocityScale * f;
    
    float4 color = 0.f;
    
    for (int i = 0; i < CAMERA_MOTION_BLUR_SAMPLE_COUNT; i++)
    {
        float2 newUV = saturate(input.Texcoord0 + velocity.xy * i);
        
        // �ӵ� �������� �̵��ϸ鼭 ���ø� (�ٵ� �̰� �ֺ� ���� ��ȭ�� ���� ���� ��ġ�� �ٸ��� �ϴ°� ���� �� ������ ..)
        float4 currentColor = Texture_Target.Sample(Sam_Clamp, newUV);
        
        color += currentColor;
    }
    
    return float4((color / CAMERA_MOTION_BLUR_SAMPLE_COUNT).xyz, 1.f);
}