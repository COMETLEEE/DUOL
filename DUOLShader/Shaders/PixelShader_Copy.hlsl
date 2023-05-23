struct VS_OUT
{
    float4 PosH : SV_POSITION;
    float2 Texcoord0 : TEXCOORD0;
};

// 샘플링할 프레임 버퍼
Texture2D Texture_Target : register(t0);

SamplerState Sam_Clamp : register(s0);

float4 PSMain(VS_OUT input) : SV_TARGET
{
    return float4(Texture_Target.Sample(Sam_Clamp, input.Texcoord0).xyz, 1.f);
}