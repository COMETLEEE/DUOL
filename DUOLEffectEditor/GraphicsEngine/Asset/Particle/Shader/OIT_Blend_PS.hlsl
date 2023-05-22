#include "ConstantBuffer.hlsli"
#include "OIT_Header.hlsli"

Texture2D gBackGround : register(t0);
Texture2D gPixelData_Over_Color_Texture : register(t2);
Texture2D gPixelData_Over_Info_Texture : register(t3);
Texture2D gPixelData_Additive_Color_Texture : register(t4);
Texture2D gPixelData_Additive_Info_Texture : register(t5);

struct VS_OUT
{
    float4 pos : SV_POSITION;
    float2 uv : UV;
};



float4 OIT_Blend_PS(VS_OUT pin) : SV_Target
{
    uint2 vPos = (uint2) pin.pos.xy;
    int numPixels = 0;
    uint offset = gScreenXY.x * vPos.y + vPos.x;
    
    float4 overColor = gPixelData_Over_Color_Texture.Sample(samAnisotropic, pin.uv);
    float4 overInfo = gPixelData_Over_Info_Texture.Sample(samAnisotropic, pin.uv);
    float4 additiveColor = gPixelData_Additive_Color_Texture.Sample(samAnisotropic, pin.uv);
    float4 additiveInfo = gPixelData_Additive_Info_Texture.Sample(samAnisotropic, pin.uv);
    float4 currColor = gBackGround.Sample(samAnisotropic, pin.uv);
    
    float overCount = overInfo.y;
    float overDepthSum = overInfo.x;
    
    float addtiveCount = additiveInfo.y;
    float addtiveDepthSum = additiveInfo.x;
    
    float alpha_mul = 1;
    alpha_mul *= overInfo.a;

    
    if (overInfo.y == 0 && additiveInfo.y == 0)
        return currColor;
    
    if (overCount != 0)
        overDepthSum /= overCount;
    if (addtiveCount != 0)
        addtiveDepthSum /= addtiveCount;
    
    float4 weightedBlendedColor = 0;
    
    if (overDepthSum < addtiveDepthSum)
        currColor += additiveColor;
    else
        weightedBlendedColor = additiveColor;
    
    overInfo.z += 0.000001f;

    currColor *= alpha_mul;
    
    alpha_mul = 1 - alpha_mul;
    
    weightedBlendedColor += overColor / overInfo.z * alpha_mul;
    
    currColor = weightedBlendedColor + currColor;
    
    return currColor;
}