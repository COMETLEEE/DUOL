#include "ConstantBuffer.hlsli"
#include "OIT_Header.hlsli"

Texture2D gBackGround : register(t0);
StructuredBuffer<PixelNode> gPieceLinkBuffer : register(t2);
ByteAddressBuffer gFirstOffsetBuffer : register(t3);

static PixelData gSortedPixels[MAX_SORTED_PIXELS];

struct VS_OUT
{
    float4 pos : SV_POSITION;
    float2 uv : UV;
};

//void SortPixelInPlace(int numPixels, PixelData gSortedPixels[MAX_SORTED_PIXELS], out PixelData outSortedPixels[MAX_SORTED_PIXELS])
//{
//    PixelData temp;
//    for (int i = 1; i < numPixels; ++i)
//    {
//        for (int j = i - 1; j >= 0; --j)
//        {
//            if (gSortedPixels[j].Depth < gSortedPixels[j + 1].Depth)
//            {
//                temp = gSortedPixels[j];
//                gSortedPixels[j] = gSortedPixels[j + 1];
//                gSortedPixels[j + 1] = temp;
                
//            }
//            else
//            {
//                break;
//            }
//        }
//    }
    
//    for (i = 0; i < numPixels; i++)
//    {
//        outSortedPixels[i] = gSortedPixels[i];
//    }
    
    
//}

float4 OIT_Blend_PS(VS_OUT pin) : SV_Target
{
    
    
    uint2 vPos = (uint2) pin.pos.xy;
    int startOffsetAddress = 4 * (gScreenXY.x * vPos.y + vPos.x);
    int numPixels = 0;
    uint offset = gFirstOffsetBuffer.Load(startOffsetAddress);
    
    PixelNode element;
    
    float4 currColor = gBackGround.Sample(samAnisotropic, pin.uv);
    
    
    while (offset != 0xFFFFFFFF)
    {
        element = gPieceLinkBuffer[offset];
        
        float4 pixelColor = UnpackColorFromUInt(element.Data.Color);
        
        if (element.Data.BlendType == 0)
            currColor.xyz = lerp(currColor.xyz, pixelColor.xyz, pixelColor.w);
        else if (element.Data.BlendType == 1)
            currColor.xyz += pixelColor.xyz * pixelColor.a;
        else
            currColor.xyz = lerp(currColor.xyz, pixelColor.xyz, pixelColor.w);
        
        
        offset = (numPixels >= MAX_SORTED_PIXELS) ?
            0xFFFFFFFF : element.Next;
    }
    
    return currColor;
}


//float4 OIT_Blend_PS(VS_OUT pin) : SV_Target
//{
//    uint2 vPos = (uint2) pin.pos.xy;
//    int startOffsetAddress = 4 * (gScreenXY.x * vPos.y + vPos.x);
//    int numPixels = 0;
//    uint offset = gFirstOffsetBuffer.Load(startOffsetAddress);
    
//    PixelNode element;
    
    
//    float3 additiveColor = 0;
    
//    float4 rgbaSum = 0;
    
//    float alpha_mul = 1;
    
//    uint prev = 0xFFFFFFFF;
    
//    float4 currColor = gBackGround.Sample(samAnisotropic, pin.uv);
    
//    while (offset != 0xFFFFFFFF)
//    {
//        element = gPieceLinkBuffer[offset];
        
//        float4 pixelColor = UnpackColorFromUInt(element.Data.Color);
        
//        // near : 1, far : 300  500/300 // 0.1~500 를 위한 Z 값이다.
//        float z = (499.9f / 299.0f) * (element.Data.Depth - 1) + 0.1f;
//        // weight값을 구하는 식
//        float w = pixelColor.w * max(0.01f,
//        min(3000.0f, 0.03f / (0.00001f + pow(abs(z) / 200.0f, 4.0f))));
        
        
//        if (element.Data.BlendType == 0)
//        {
//            rgbaSum += float4((pixelColor.xyz + additiveColor) * pixelColor.a, pixelColor.a) * w;
//            alpha_mul *= 1 - pixelColor.a;
//            additiveColor = float3(0.0f, 0.0f, 0.0f);

//        }
//        else if (element.Data.BlendType == 1)
//        {
//            if (element.Next != 0xFFFFFFFF)
//                additiveColor += pixelColor.xyz * pixelColor.a * w;
//            else if (prev == 0xFFFFFFFF)
//                currColor += float4((pixelColor.xyz) * pixelColor.a, pixelColor.a);
//            else
//            {
//                rgbaSum += float4((pixelColor.xyz) * pixelColor.a, pixelColor.a) * w;
//                alpha_mul *= 1 - pixelColor.a;
//            }
                
//        }
        
//        offset = (numPixels >= MAX_SORTED_PIXELS) ?
//            0xFFFFFFFF : element.Next;
//    }
    
//    rgbaSum.a += 0.000001f;

//    currColor *= alpha_mul;
    
//    alpha_mul = 1 - alpha_mul;
    
//    float4 weightedBlendedColor = rgbaSum / rgbaSum.a * alpha_mul;
    
//    currColor = weightedBlendedColor + currColor;
    
//    return currColor;

//}