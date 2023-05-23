Texture2D DebugTexture;
Texture2D RenderTargetDepth;
Texture2D DebugDepth;


struct VertexOut
{
    float4 PosH : SV_POSITION;
    float2 UV : TEXCOORD0;
};

SamplerState g_samLinear : register(s0);

float4 PSMain(VertexOut pin) : SV_Target
{
    float ColorD = RenderTargetDepth.Sample(g_samLinear, pin.UV).r;
    float4 DebugColor = DebugTexture.Sample(g_samLinear, pin.UV);
    float DebugColorD = DebugDepth.Sample(g_samLinear, pin.UV).r;

    if (DebugColorD < ColorD)
    {
        return DebugColor;
    }
    else
    {
        clip(-1);
    }

    return float4(0.f, 0.f, 0.f, 0.f);
}
