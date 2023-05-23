#include "ConstantBuffer.hlsli"

Texture2D OutlineTexture;

struct VertexOut
{
    float4 PosH : SV_POSITION;
    float2 UV : TEXCOORD0;
};

SamplerState g_samLinear : register(s0);

float4 PSMain(VertexOut pin) : SV_Target
{
    float4 color = OutlineTexture.Sample(g_samLinear, pin.UV);

    // 검정색이면 ㅃㅇ ..!
    if ((color.x == 0.f) && (color.y == 0.f) && (color.z == 0.f) && (color.w == 0.f))
        clip(-1);

    return color;
}