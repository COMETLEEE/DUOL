#include "ConstantBuffer.hlsli"

#define MAX_BONE_TRANSFORM_COUNT 128
// #define USE_SKINNING
//--------------------------------------------------------------------------------------
//  Globals
//--------------------------------------------------------------------------------------
//// PerObject
#ifdef USE_SKINNING
cbuffer cbPerObject : register(b2)
{
    int g_idx;
    int g_isStatic;
    float2 g_pad1;
    float4 g_EffectInfo;

    Transfrom g_Transform;                             // : packoffset(c0);
    matrix g_BoneTransforms[MAX_BONE_TRANSFORM_COUNT]; // : packoffset(c8);
    Material g_Material;                               // : packoffset(c520); //8 + 4 * 128
};
#else
cbuffer cbPerObject : register(b2)
{
    int g_idx;
    int g_isStatic;
    int g_isAlphaClip;
    int g_isAlphaClip;
    float4 g_EffectInfo;

    Transfrom g_Transform; //: packoffset(c0);
    Material g_Material;   //: packoffset(c8);
}
#endif
//--------------------------------------------------------------------------------------
// Input / Output structures
//--------------------------------------------------------------------------------------
#ifdef USE_SKINNING
struct VS_INPUT
{
    float3 Position : POSITION;
    float2 Texcoord0 : TEXCOORD0;
    float3 Normal : NORMAL;
    float3 Tangent : TANGENT;
    int BoneIndex[8] : BONEINDEX;
    float Weight[8] : WEIGHT;
};
#else
struct VS_INPUT
{
    float3 Position : POSITION;
    float2 Texcoord0 : TEXCOORD0;
    float3 Normal : NORMAL;
    float3 Tangent : TANGENT;
};
#endif

struct VS_OUTPUT
{
    float4 PosH : SV_POSITION;
    float2 Texcoord0 : TEXCOORD0;
};

struct GS_OUTPUT
{
    float4 PosH : SV_POSITION;
    float2 Texcoord0 : TEXCOORD0;
    uint RTIndex : SV_RenderTargetArrayIndex;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VSMain(VS_INPUT Input)
{
    VS_OUTPUT Output;

#ifdef USE_SKINNING
    float3 posL = float3(0.0f, 0.0f, 0.0f);
    float3 normalL = float3(0.0f, 0.0f, 0.0f);
    float3 tangentL = float3(0.0f, 0.0f, 0.0f);

    float totalWeight = 0;

    [unroll]
    for (int i = 0; i < 8; i++)
    {
        if (Input.BoneIndex[i] > -1)
        {
            totalWeight += Input.Weight[i];
            posL += Input.Weight[i] * mul(float4(Input.Position, 1.0f), g_BoneTransforms[Input.BoneIndex[i]]).xyz;
        }
    }

    posL /= totalWeight;
    Output.PosH = float4(posL.xyz, 1.f);
#else
    Output.PosH = mul(float4(Input.Position, 1.0f), g_Transform.g_World);
#endif

    Output.Texcoord0 = Input.Texcoord0;

    return Output;
}

//Cascade Shadow
//--------------------------------------------------------------------------------------
// Geometry Shader
//--------------------------------------------------------------------------------------
[maxvertexcount(12)] // 4(shadow slice) * 3
void GSMain(triangle VS_OUTPUT input[3], inout TriangleStream<GS_OUTPUT> stream)
{
    GS_OUTPUT output;

    for (int cascade = 0; cascade < 4; ++cascade)
    {
        //Generate cascade slice
        output.RTIndex = cascade;

        for (int i = 0; i < 3; ++i)
        {
            output.PosH = input[i].PosH;
            output.PosH = mul(output.PosH, g_shadow.Shadow[cascade]);
            output.Texcoord0 = input[i].Texcoord0;
            //output.PosH /= output.PosH.w; //orth이므로 w는 어짜피 1이다.
            stream.Append(output);
        }

        stream.RestartStrip();
    }
}

//Spot Shadow
//--------------------------------------------------------------------------------------
// Geometry Shader
//--------------------------------------------------------------------------------------
[maxvertexcount(3)] // 1(shadow slice) * 3
void SpotGSMain(triangle VS_OUTPUT input[3], inout TriangleStream<GS_OUTPUT> stream)
{


    for (int i = 0; i < 3; ++i)
    {
        GS_OUTPUT output;
        // Generate cascade slice
        if(g_isStatic > 0)
            output.RTIndex = g_Light[g_idx].staticShadowMapIdx;
        else
            output.RTIndex = g_Light[g_idx].dynamicShadowMapIdx;

        output.PosH = input[i].PosH;
        output.PosH = mul(output.PosH, g_Light[g_idx].ShadowMatrix[0]);
        output.Texcoord0 = input[i].Texcoord0;
        //output.PosH /= output.PosH.w;
        //output.PosH.w = 1.f;
        stream.Append(output);
    }

    stream.RestartStrip();
}

// PointLight Shadow
//--------------------------------------------------------------------------------------
//  Geometry Shader
//--------------------------------------------------------------------------------------
[maxvertexcount(18)] // 6(shadow slice) * 3
void PointGSMain(triangle VS_OUTPUT input[3], inout TriangleStream<GS_OUTPUT> stream)
{

    for (int directionIdx = 0; directionIdx < 6 ;directionIdx++)
        {

            for (int i = 0; i < 3; ++i)
            {
                GS_OUTPUT output;

                // 현재의 라이트맵 포지션 + 그릴 평면
                if (g_isStatic > 0)
                    output.RTIndex = g_Light[g_idx].staticShadowMapIdx * 6 + directionIdx;
                else
                    output.RTIndex = g_Light[g_idx].dynamicShadowMapIdx * 6 + directionIdx;

                output.PosH = input[i].PosH;
                output.PosH = mul(output.PosH, g_Light[g_idx].ShadowMatrix[directionIdx]);
                output.Texcoord0 = input[i].Texcoord0;

                stream.Append(output);
        }
          stream.RestartStrip();
    }

}

//--------------------------------------------------------------------------------------
// Textures and Samplers
//--------------------------------------------------------------------------------------
Texture2D g_DiffuseTexture : register(t0);
Texture2D g_NormalMapTexture : register(t1);

SamplerState g_samLinear : register(s0);

//--------------------------------------------------------------------------------------
// Input / Output structures
//--------------------------------------------------------------------------------------
struct PS_INPUT
{
    float4 PosH : SV_POSITION;
    float2 Texcoord0 : TEXCOORD0;
    uint RTIndex : SV_RenderTargetArrayIndex;
};

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
void PSMain(PS_INPUT Input)
{
    float4 diffuse = g_DiffuseTexture.Sample(g_samLinear, Input.Texcoord0);
    bool isAlpha = (g_Material.Albedo.w < 0.8f);
    bool isAlpha2 = (diffuse.w < 0.8f);
    if (isAlpha && isAlpha2)
    {
          clip(-1.f);
    }

    return;
}
