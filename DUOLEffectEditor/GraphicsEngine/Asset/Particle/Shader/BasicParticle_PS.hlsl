#include "ConstantBuffer.hlsli"
#include "OIT_Header.hlsli"

Texture2DArray gTexArray : register(t0);

Texture2D gDepthBuffer : register(t1); // 파티클을 그릴 때 앞에 오브젝트가 있으면 그리지 않기 위해서 뎁스버퍼를 참조한다.
// 기존의 뎁스 값, 불투명한 오브젝트를 그린 뎁스값이다.
Texture2D gPreDepthBuffer : register(t2); // 파티클을 그릴 때 앞에 오브젝트가 있으면 그리지 않기 위해서 뎁스버퍼를 참조한다.
// OIT Layer를 그릴 때 생긴 뎁스 값이다.

RWStructuredBuffer<PixelNode> gPixelLinkBuffer : register(u0); // 정적 리스트의 저장소 역할을 하는 버퍼. 1920 * 1080 * 저장할 픽셀 수 의 사이즈로 할당해둔다.
RWByteAddressBuffer gFirstOffsetBuffer : register(u1); // 인덱스 버퍼. 마지막으로 기록된 픽셀의 인덱스 값을 저장하고 있다. 리스트 형식으로 저장된 픽셀을 추적한다.

struct PSOut
{
    float4 Color : SV_Target;
    float Depth : SV_Depth;
};
struct GeoOut
{
    float4 PosH : SV_Position;
    float4 Color : COLOR;
    float2 Tex : TEXCOORD1;
};
//#define Draw_Depth 1

float4 DrawPS(GeoOut pin) : SV_Target
{
    const float2 posTexCoord = pin.PosH.xy / gScreenXY;

    float4 depth = gDepthBuffer.Sample(samAnisotropic, posTexCoord);

    float4 Color = gTexArray.Sample(samAnisotropic, float3(pin.Tex, 0)) * pin.Color;
    
    if ((depth.x >= 0.001f))
    {
        clip(depth.x - pin.PosH.z - EPSILON);
    }
    
    clip(Color.a - EPSILON);
    
#ifdef Draw_Object_ID
    return float4(gCommonInfo.gObjectID, 0, 0, 1.0f);
#elif Draw_Depth
    return float4(pin.PosH.z, pin.PosH.z, pin.PosH.z, pin.PosH.z);
#else
    return Color;
#endif
}
float4 DrawOutLine(GeoOut pin) : SV_Target
{
    return float4(1.0f, 1.0f, 0.0f, 1.0f);
}

PSOut DrawDepthPeelingPS(GeoOut pin) // 1차 적으로 구현했던 뎁스필링 방식의 OIT 최적화를 위해 버린다..!
{
    const float2 posTexCoord = pin.PosH.xy / gScreenXY;

    float curDepth = pin.PosH.z;

    float4 depth = gDepthBuffer.Sample(samAnisotropic, posTexCoord);

    if ((depth.x >= EPSILON))
    {
        clip(depth.x - curDepth - EPSILON);
    }
    float preDepth = gPreDepthBuffer.Sample(samAnisotropic, posTexCoord).x;

    clip(curDepth - preDepth - EPSILON); // 이전 레이어의 뎁스와 비교하기 위함. 더욱 뒤에 있을 경우에 그리고 앞에 있다면 더 이상 그리지 않아도 된다.

    PSOut output;

    output.Color = gTexArray.Sample(samAnisotropic, float3(pin.Tex, 0)) * pin.Color;

    clip(output.Color.a - EPSILON);

    output.Depth = curDepth;

    return output;

}

void OIT_Particle_PS(GeoOut pin) // 픽셀을 저장하는 pixel shader
{
    float4 texColor = gTexArray.Sample(samAnisotropic, float3(pin.Tex, 0)) * pin.Color;
    
    const float2 posTexCoord = pin.PosH.xy / gScreenXY;

    float4 depth = gDepthBuffer.Sample(samAnisotropic, posTexCoord);
    
    clip(texColor.a - 0.0001f);

    if ((depth.x >= 0.001f))
    {
        clip(depth.x - pin.PosH.z - EPSILON);
    }
    
    uint pixelCount = gPixelLinkBuffer.IncrementCounter(); // 카운트를 기록한다.
    
    uint2 vPos = (uint2) pin.PosH.xy;
    
    uint startOffsetAddress = 4 * (gScreenXY.x * vPos.y + vPos.x);
    uint oldStartOffset;
    
    // FirstOffsetBuffer는 화면에 마지막으로 기록된 픽셀의 인덱스 값을 저장하는 버퍼이다..
    gFirstOffsetBuffer.InterlockedExchange(
        startOffsetAddress, pixelCount, oldStartOffset);
    
    float strength = length(texColor);
    float4 color = texColor;
    
    PixelNode node;
    node.Data.Color = PackColorFromFloat4(color);
    node.Data.Depth = pin.PosH.z;
    node.Data.BlendType = gParticleRenderer.gBlendType;
    node.Next = oldStartOffset;
    
    gPixelLinkBuffer[pixelCount] = node;
}