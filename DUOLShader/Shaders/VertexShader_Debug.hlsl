#include "ConstantBuffer.hlsli"

//텍스쳐병함
struct VertexIn
{
    float3 Pos : POSITION; //로컬좌표
    uint Color : Color;
};

struct VertexOut
{
    float4 PosH : SV_POSITION;
    uint Color : Color;
};

VertexOut VSMain(VertexIn vin)
{
    VertexOut vout;
    
    vout.PosH = mul(float4(vin.Pos, 1.0f), g_Camera.g_ViewProjectionMatrix);
    vout.Color = vin.Color;
    
    return vout;
}
