#include "ConstantBuffer.hlsli"
//--------------------------------------------------------------------------------------
// File: BasicCompute11.hlsl
//
// This file contains the Compute Shader to perform array A + array B
// 
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

struct BoundingBox
{
    float3 _extent;
    float pad1;
    float3 _position;
    float pad2;
};

StructuredBuffer<BoundingBox> BufferIn : register(t0);
Texture2D OccluderDepth : register(t1);

struct DebugData
{
    float4 screenSpaceExtent;
    float4 screenSpaceCenter;
    float4 sampleeee;
    float maxDepth;
    float closestDepth;
    float LOD;
    float ID;
};

SamplerState Linear : register(s0);

RWStructuredBuffer<DebugData> BufferOut : register(u0);
//RWStructuredBuffer<> BufferOut : register(u0);

[numthreads(64, 1, 1)]
void CSMain( uint3 DTid : SV_DispatchThreadID )
{
    float3 extents[8];

    //axis-aligned bounding 박스의 포지션을 월드스페이스로
    extents[0] = BufferIn[DTid.x]._position + BufferIn[DTid.x]._extent;
    extents[1] = BufferIn[DTid.x]._position + float3(-BufferIn[DTid.x]._extent.x, +BufferIn[DTid.x]._extent.y, +BufferIn[DTid.x]._extent.z);
    extents[2] = BufferIn[DTid.x]._position + float3(+BufferIn[DTid.x]._extent.x, -BufferIn[DTid.x]._extent.y, +BufferIn[DTid.x]._extent.z);
    extents[3] = BufferIn[DTid.x]._position + float3(+BufferIn[DTid.x]._extent.x, +BufferIn[DTid.x]._extent.y, -BufferIn[DTid.x]._extent.z);
    extents[4] = BufferIn[DTid.x]._position + float3(-BufferIn[DTid.x]._extent.x, +BufferIn[DTid.x]._extent.y, -BufferIn[DTid.x]._extent.z);
    extents[5] = BufferIn[DTid.x]._position + float3(-BufferIn[DTid.x]._extent.x, -BufferIn[DTid.x]._extent.y, +BufferIn[DTid.x]._extent.z);
    extents[6] = BufferIn[DTid.x]._position + float3(+BufferIn[DTid.x]._extent.x, -BufferIn[DTid.x]._extent.y, -BufferIn[DTid.x]._extent.z);
    extents[7] = BufferIn[DTid.x]._position + float3(-BufferIn[DTid.x]._extent.x, -BufferIn[DTid.x]._extent.y, -BufferIn[DTid.x]._extent.z);

    float4 screenSpaceExtents[8];

    //NDC좌표계로 옮긴다
    [unroll]
    for(int idx = 0; idx < 8; idx++)
    {
        screenSpaceExtents[idx] = mul(float4( extents[idx], 1.f), g_Camera.g_ViewProjectionMatrix);
        screenSpaceExtents[idx].xyz /= screenSpaceExtents[idx].w;
        
        //-1~1사이의 값을 0과 1사이로.

        screenSpaceExtents[idx].xy += 1.f;
        screenSpaceExtents[idx].xy /= 2.f;
    }

    float2 maxScreenSpaceExtent = { 0.f, 0.f };
    float closestDepth = 1.f;

    //중앙좌표또한 NDC좌표계로 옮겨준다
    float4 screenSpaceCenterPos = mul(float4(BufferIn[DTid.x]._position, 1.f), g_Camera.g_ViewProjectionMatrix);

    screenSpaceCenterPos.xyz /= screenSpaceCenterPos.w;
    //-1~1사이의 값을 0과 1사이로.
    screenSpaceCenterPos.xy += 1.f;
    screenSpaceCenterPos.xy /= 2.f;

    //rect구하기
    [unroll]
    for (int idx2 = 0; idx2 < 8; idx2++)
    {
        float2 dist = screenSpaceExtents[idx2].xy - screenSpaceCenterPos.xy;
        dist = abs(dist);

        maxScreenSpaceExtent = max(maxScreenSpaceExtent, dist);
        closestDepth = min(closestDepth, screenSpaceExtents[idx2].z);
    }

    float viewX = (maxScreenSpaceExtent.x * 2) * g_ScreenXY.x;
    float viewY = (maxScreenSpaceExtent.y * 2) * g_ScreenXY.y;

    float LOD = ceil(log2(max(viewX, viewY) / 2.f));

    float4 depthSample;

    float2 samplePos = screenSpaceCenterPos.xy + maxScreenSpaceExtent.xy;
    samplePos.y = 1.0f - samplePos.y;
    depthSample.x = OccluderDepth.SampleLevel(Linear, samplePos, LOD);
    samplePos = float2(screenSpaceCenterPos.x + maxScreenSpaceExtent.x, screenSpaceCenterPos.y -  maxScreenSpaceExtent.y);
    samplePos.y = 1.0f - samplePos.y;
    depthSample.y = OccluderDepth.SampleLevel(Linear, samplePos, LOD);
    samplePos = float2(screenSpaceCenterPos.x - maxScreenSpaceExtent.x, screenSpaceCenterPos.y +  maxScreenSpaceExtent.y);
    samplePos.y = 1.0f - samplePos.y;
    depthSample.z = OccluderDepth.SampleLevel(Linear, samplePos, LOD);
    samplePos = screenSpaceCenterPos.xy - maxScreenSpaceExtent;
    samplePos.y = 1.0f - samplePos.y;
    depthSample.w = OccluderDepth.SampleLevel(Linear, samplePos, LOD);

    float maxDepth = max(max(depthSample.x, depthSample.y),  max(depthSample.z, depthSample.w));

    //물체의 가장 가까운 값이 샘플링한 뎁스값중 가장 멀리있는 값보다 크면(더 멀리있으면) 컬링한다.
    BufferOut[DTid.x].screenSpaceCenter = screenSpaceCenterPos;
    BufferOut[DTid.x].screenSpaceExtent.xy = maxScreenSpaceExtent.xy;
    BufferOut[DTid.x].screenSpaceExtent.zw = float2(viewX, viewY);

    // BufferOut[DTid.x].sampleeee.xy = screenSpaceCenterPos.xy + maxScreenSpaceExtent;
    // BufferOut[DTid.x].sampleeee.zw = float2(screenSpaceCenterPos.x + maxScreenSpaceExtent.x, screenSpaceCenterPos.y -  maxScreenSpaceExtent.y);

    BufferOut[DTid.x].sampleeee = depthSample;
    BufferOut[DTid.x].closestDepth = closestDepth;
    BufferOut[DTid.x].maxDepth = maxDepth;
    BufferOut[DTid.x].LOD = LOD;
    BufferOut[DTid.x].ID  = (closestDepth > maxDepth)? 0.f : 1.f ;
}