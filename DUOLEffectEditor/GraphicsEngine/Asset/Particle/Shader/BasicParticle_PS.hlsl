#include "ConstantBuffer.hlsli"
#include "OIT_Header.hlsli"

Texture2DArray gTexArray : register(t0);

Texture2D gDepthBuffer : register(t1); // 파티클을 그릴 때 앞에 오브젝트가 있으면 그리지 않기 위해서 뎁스버퍼를 참조한다.
// 기존의 뎁스 값, 불투명한 오브젝트를 그린 뎁스값이다.
Texture2D gPreDepthBuffer : register(t2); // 파티클을 그릴 때 앞에 오브젝트가 있으면 그리지 않기 위해서 뎁스버퍼를 참조한다.
// OIT Layer를 그릴 때 생긴 뎁스 값이다.

struct PSOut
{
    float4 Color : SV_Target;
    float Depth : SV_Depth;
};
struct OITOut
{
    float4 OverColor : SV_Target0;
    float4 OverInfo : SV_Target1;
    float4 AdditiveColor : SV_Target2;
    float4 AdditiveInfo : SV_Target3;
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

OITOut OIT_Particle_PS(GeoOut pin) // 픽셀을 저장하는 pixel shader
{
    OITOut oitOut;
    
    oitOut.OverColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
    oitOut.OverInfo = float4(0.0f, 0.0f, 0.0f, 1.0f);
    oitOut.AdditiveColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
    oitOut.AdditiveInfo = float4(0.0f, 0.0f, 0.0f, 1.0f);
    
    float4 texColor = gTexArray.Sample(samAnisotropic, float3(pin.Tex, 0)) * pin.Color;
    
    const float2 posTexCoord = pin.PosH.xy / gScreenXY;

    float4 depth = gDepthBuffer.Sample(samAnisotropic, posTexCoord);
    
    clip(texColor.a - 0.0001f);

    if ((depth.x >= 0.001f))
    {
        clip(depth.x - pin.PosH.z - EPSILON);
    }
    
    uint2 vPos = (uint2) pin.PosH.xy;
    
    uint offset = gScreenXY.x * vPos.y + vPos.x;
    
    float strength = length(texColor);
    float4 color = texColor;
    
    float currentDepth = pin.PosH.z * pin.PosH.w;
    
    if (gParticleRenderer.gBlendType == 0) // Over
    {
        // near : 1, far : 300  500/300 // 0.1~500 를 위한 Z 값이다.
        float z = currentDepth;
        float w = color.w * max(0.01f, min(3000.0f, 0.03f / (0.00001f + pow(abs(z) / 200.0f, 4.0f))));
        
        if (color.w >= 0.999f)
            w *= 2;
        
        oitOut.OverColor = float4((color.xyz) * color.a, color.a) * w;
        oitOut.OverInfo.x = currentDepth;
        oitOut.OverInfo.y = 1.0f;
        oitOut.OverInfo.z = color.a * w;
        oitOut.OverInfo.a = 1 - color.a;
    }
    else // Additive
    {
        oitOut.AdditiveColor = float4((color.xyz) * color.a, color.a);
        oitOut.AdditiveInfo.x = currentDepth;
        oitOut.AdditiveInfo.y = 1.0f;
        oitOut.AdditiveInfo.z = color.a;
        oitOut.AdditiveInfo.a = 1 - color.a;
    }
    
    return oitOut;
}