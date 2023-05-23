Texture2D gRenderTargetTexture;

struct VertexOut
{
    float4 PosH : SV_POSITION;
    uint Color : Color;
};

SamplerState g_samLinear : register(s0);

float4 PSMain(VertexOut pin) : SV_Target
{ 
    uint rInt = ((0xff000000 & pin.Color) >> 24);
    float r = float(rInt) / 255.f;
    
    uint gInt = ((0x00ff0000 & pin.Color) >> 16);
    float g = float(gInt) / 255.f;
    
    uint bInt = ((0x0000ff00 & pin.Color) >> 8);
    float b = float(bInt) / 255.f;
    
    uint aInt = (0x000000ff & pin.Color);
    float a = float(aInt) / 255.f;

    return float4(r, g, b, a);
}