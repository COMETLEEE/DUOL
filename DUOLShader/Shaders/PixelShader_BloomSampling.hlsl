
struct VertexOut
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
};

struct Bloom
{
    float intensity;
    float threshhold;
};

cbuffer cbPerObject : register(b2)
{
    Bloom g_Bloom;
};

Texture2D g_InputTexture : register(t0);

SamplerState g_samLinear :register(s0);

float GetBloomCurve(float intensity, float threshold)
{
    float result = intensity;
    intensity *= 2.0f;

#ifdef BLOOMCURVE_METHOD_1
    result = intensity * 0.05 + max(0, intensity - threshold) * 0.5; //default threshold =1.26
#endif

#ifdef BLOOMCURVE_METHOD_2
    result = intensity * intensity / 3.2;
#endif

#ifdef BLOOMCURVE_METHOD_3
    result = max(0, intensity - threshold); //default threshold =1.0
    result *= result;
#endif

    return result * 0.5f;
}

float4 PSMain(VertexOut input) : SV_Target
{
    float4 color = g_InputTexture.Sample(g_samLinear,input.uv);

    //float intensity = dot(color.rgb, float3(0.3f, 0.3f, 0.3f));
    float intensity = dot(color.rgb, float3(g_Bloom.intensity.rrr));

    // float bloom_Intensity = GetBloomCurve(intensity, 1.26f);
    float bloom_Intensity = GetBloomCurve(intensity, g_Bloom.threshhold);

    // 강도가 0으로 나오면 문제가 생길수 있다.
    if (intensity == (float)0.f)
        return float4(0.f, 0.f, 0.f, 0.f);

    return color * bloom_Intensity / intensity;
}
