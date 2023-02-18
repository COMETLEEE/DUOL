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

void SortPixelInPlace(int numPixels)
{
    PixelData temp;
    for (int i = 1; i < numPixels; ++i)
    {
        for (int j = i - 1; j >= 0; --j)
        {
            if (gSortedPixels[j].Depth < gSortedPixels[j + 1].Depth)
            {
                temp = gSortedPixels[j];
                gSortedPixels[j] = gSortedPixels[j + 1];
                gSortedPixels[j + 1] = temp;
            }
            else
            {
                break;
            }
        }
    }
}

float4 OIT_Blend_PS(VS_OUT pin) : SV_Target
{
    uint2 vPos = (uint2) pin.pos.xy;
    int startOffsetAddress = 4 * (gScreenXY.x * vPos.y + vPos.x);
    int numPixels = 0;
    uint offset = gFirstOffsetBuffer.Load(startOffsetAddress);
    
    PixelNode element;
    
    while (offset != 0xFFFFFFFF)
    {
        element = gPieceLinkBuffer[offset];
        gSortedPixels[numPixels++] = element.Data;
        offset = (numPixels >= MAX_SORTED_PIXELS) ?
            0xFFFFFFFF : element.Next;
    }
    
    SortPixelInPlace(numPixels);
    
    float4 currColor = gBackGround.Sample(samAnisotropic, pin.uv);
    
    PixelData data;
    
	[unroll]
    for (int i = 0; i < numPixels; ++i)
    {
        data = gSortedPixels[i];
        float4 pixelColor = UnpackColorFromUInt(data.Color);
        
        if (data.BlendType == 0)
            currColor.xyz = lerp(currColor.xyz, pixelColor.xyz, pixelColor.w);
        else if (data.BlendType == 1)
            currColor.xyz = lerp(currColor.xyz, pixelColor.xyz + currColor.xyz, pixelColor.w);
        else
            currColor.xyz = lerp(currColor.xyz, pixelColor.xyz, pixelColor.w);
    }
    
    return currColor;
}