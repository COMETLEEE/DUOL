#include "ConstantBuffer.hlsli"

Texture2DArray gTexArray : register(t0);

Texture2D gDepthBuffer : register(t1); // 파티클을 그릴 때 앞에 오브젝트가 있으면 그리지 않기 위해서 뎁스버퍼를 참조한다.

struct GeoOut
{
    float4 PosH : SV_Position;
    float4 Color : COLOR;
    float2 Pos : TEXCOORD1; // NDC공간의 좌표 값.
    float2 Tex : TEXCOORD2;
};

float4 DrawPS(GeoOut pin) : SV_Target
{

    float2 posTexCoord = pin.Pos.xy;

    posTexCoord = pin.PosH.xy / gScreenXY;
    float4 depth = gDepthBuffer.Sample(samAnisotropic, posTexCoord);

    if ((depth.x >= 0.001f) && (depth.x - pin.PosH.z < 0.001f))
    {
        clip(-1.f);
    }

    return gTexArray.Sample(samAnisotropic, float3(pin.Tex, 0)) * pin.Color;
}
