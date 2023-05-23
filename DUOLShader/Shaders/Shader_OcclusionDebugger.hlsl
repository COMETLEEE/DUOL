#include "ConstantBuffer.hlsli"

#define MAX_BONE_TRANSFORM_COUNT 128
// #define USE_SKINNING
//--------------------------------------------------------------------------------------
//  Globals
//--------------------------------------------------------------------------------------
//// PerObject

//--------------------------------------------------------------------------------------
// Input / Output structures
//--------------------------------------------------------------------------------------
struct VS_INPUT
{
    float4 screenSpaceExtent : POSITION;
    float4 screenSpaceCenter : NORMAL;
    float maxDepth : PSIZE0;
    float closestDepth : PSIZE1;
    float LOD : PSIZE2;
    float ID : PSIZE3;
};

struct VS_OUTPUT
{
    float4 PosH : SV_POSITION;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_INPUT VSMain(VS_INPUT Input)
{
    return Input
}
//--------------------------------------------------------------------------------------
// Geometry Shader
//--------------------------------------------------------------------------------------
[maxvertexcount(8)] 
void GSMain(point VS_INPUT Input, inout LineStream<VS_OUTPUT> stream)
{
    VS_OUTPUT ret = float4{0.f, 0.f, 0.f, 1.f};

    float2 centerpos = Input.xy;

    centerpos.xy = Input.screenSpaceCenter.xy;
    centerpos.xy *= 2.f;
    centerpos.xy -= 1.f;

    float2 extent =  Input.screenSpaceExtent.xy;
    extent.xy *= 2.f;
    extent.xy -= 1.f;

    float2 rect[4];

    rect[0] = centerpos + extent;
    rect[1] = float2(centerpos.x - extent.x, centerpos.y + extent.y);
    rect[2] = float2(centerpos.x + extent.x, centerpos.y - extent.y);
    rect[3] = centerpos - extent;

    ret.xy = rect[0];
    stream.Append(ret);
    ret.xy = rect[1];
    stream.Append(ret);
    stream.RestartStrip();

    ret.xy = rect[1];
    stream.Append(ret);
    ret.xy = rect[2];
    stream.Append(ret);
    stream.RestartStrip();

    ret.xy = rect[2];
    stream.Append(ret);
    ret.xy = rect[3];
    stream.Append(ret);
    stream.RestartStrip();

    ret.xy = rect[3];
    stream.Append(ret);
    ret.xy = rect[0];
    stream.Append(ret);
    stream.RestartStrip();
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
void PSMain(VS_OUTPUT Input)
{
    return float4(1.0f, 1.0f, 1.0f, 1.0f);
}
