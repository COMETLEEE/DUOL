#include "ConstantBuffer.hlsli"

//--------------------------------------------------------------------------------------
// Globals
//--------------------------------------------------------------------------------------
//// PerObject
cbuffer cbPerObject : register(b2)
{
    float2 dimension;
};

SamplerState g_samPoint : register(s0);

Texture2D g_preMipDepth;

struct VertexOut
{
    float4 PosH : SV_POSITION;
    float2 Texcoord0 : TEXCOORD0;
};

float PSMain(VertexOut pin)
    : SV_Target
{
    // uint2 res;
    // uint lev;
    // g_preMipDepth.GetDimensions(0, res.x, res.y, lev);

    // int2 pixelPos = floor(pin.PosH.xy) * 2;

    // float2 deltaPos = float2(1.f / dimension.x, 1.f / dimension.y);
    // deltaPos = pixelPos * deltaPos;
    // int2 pos[4] =
    //     {
    //         int2(+0, +0), int2(+1, +0), int2(+0, +1), int2(+1, +1)
    //     };

    // float4 sampleValue = {0.f, 0.f, 0.f, 0.f};

    // sampleValue.x = g_preMipDepth.Sample(g_samPoint, deltaPos, pos[0]).r;
    // sampleValue.y = g_preMipDepth.Sample(g_samPoint, deltaPos, pos[1]).r;
    // sampleValue.z = g_preMipDepth.Sample(g_samPoint, deltaPos, pos[2]).r;
    // sampleValue.w = g_preMipDepth.Sample(g_samPoint, deltaPos, pos[3]).r;
    // // sampleValue.x = g_preMipDepth.Load(int3(pixelPos, 0.f), pos[0]).r;
    // // sampleValue.y = g_preMipDepth.Load(int3(pixelPos, 0.f), pos[1]).r;
    // // sampleValue.z = g_preMipDepth.Load(int3(pixelPos, 0.f), pos[2]).r;
    // // sampleValue.w = g_preMipDepth.Load(int3(pixelPos, 0.f), pos[3]).r;


    // float3 extra =  {0.f, 0.f, 0.f};

    // int2 size = dimension.xy;

    // // 이전 mip해상도의 사이즈에 따라 샘플링을 다르게해주어야합니다.
    // // x가 odd일때에는..
    // if ((size.x & 1 != 0) && (int(pixelPos.x) == size.x - 3))
    // {
    //     // 둘다 odd일때는.. 오른쪽아래 모서리란 뜻입니다.
    //     if ((size.y & 1 != 0) && (int(pixelPos.y) == size.y - 3))
    //     {
    //         extra.x = g_preMipDepth.Sample(g_samPoint, deltaPos, int2(2, 2)).r;
    //     }

    //     extra.y = g_preMipDepth.Sample(g_samPoint, deltaPos, int2(2, +0)).r;
    //     extra.z = g_preMipDepth.Sample(g_samPoint, deltaPos, int2(2, +1)).r;
    // }
    // else
    // {
    //     // 혹은 y만 odd일때!
    //     if ((size.y & 1 != 0) && (int(pixelPos.y) == size.y - 3))
    //     {
    //         extra.x = extra.x, g_preMipDepth.Sample(g_samPoint, deltaPos, int2(+0, +2)).r;
    //         extra.y = extra.y, g_preMipDepth.Sample(g_samPoint, deltaPos, int2(+1, +2)).r;
    //     }
    // }

    // float maxVal = 
    // max(0.f, 
    //     max(
    //         max(
    //             max(sampleValue.x, sampleValue.y), 
    //             max(sampleValue.z, sampleValue.w)), 
    //         max(
    //             max(extra.x, extra.y), extra.z)));

    // return maxVal;

    int2 pixelPos = floor(pin.PosH.xy) * 2;

    float2 deltaPos = float2(1.f / dimension.x, 1.f / dimension.y);
    deltaPos = pixelPos * deltaPos;
    int2 pos[4] =
        {
            int2(+0, +0), int2(+1, +0), int2(+0, +1), int2(+1, +1)
        };

    float4 sampleValue = { 0.f, 0.f, 0.f, 0.f };

    sampleValue.x = g_preMipDepth.Load(int3(pixelPos, 0), pos[0]).r;
    sampleValue.y = g_preMipDepth.Load(int3(pixelPos, 0), pos[1]).r;
    sampleValue.z = g_preMipDepth.Load(int3(pixelPos, 0), pos[2]).r;
    sampleValue.w = g_preMipDepth.Load(int3(pixelPos, 0), pos[3]).r;

    float3 extra = { 0.f, 0.f, 0.f };

    int2 size = dimension.xy;

    // 이전 mip해상도의 사이즈에 따라 샘플링을 다르게해주어야합니다.
    // x가 odd일때에는..
    if ((size.x & 1 != 0) && (int(pixelPos.x) == size.x - 3))
    {
        // 둘다 odd일때는.. 오른쪽아래 모서리란 뜻입니다.
        if ((size.y & 1 != 0) && (int(pixelPos.y) == size.y - 3))
        {
            extra.x = g_preMipDepth.Load(int3(pixelPos, 0), int2(2, 2)).r;
        }

        extra.y = g_preMipDepth.Load(int3(pixelPos, 0), int2(2, +0)).r;
        extra.z = g_preMipDepth.Load(int3(pixelPos, 0), int2(2, +1)).r;
    }
    else
    {
        // 혹은 y만 odd일때!
        if ((size.y & 1 != 0) && (int(pixelPos.y) == size.y - 3))
        {
            extra.x = extra.x, g_preMipDepth.Load(int3(pixelPos, 0), int2(+0, +2)).r;
            extra.y = extra.y, g_preMipDepth.Load(int3(pixelPos, 0), int2(+1, +2)).r;
        }
    }

    float maxVal =
        max(0.f,
            max(
                max(
                    max(sampleValue.x, sampleValue.y),
                    max(sampleValue.z, sampleValue.w)),
                max(
                    max(extra.x, extra.y), extra.z)));

    return maxVal;
}
