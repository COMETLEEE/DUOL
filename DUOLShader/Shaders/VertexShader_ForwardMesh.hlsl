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
    float2 g_ObjectID;
    float g_Offset; // �ż��� �߰�. PaperBurn �� ���� Offset��.
    float g_renderFlag;

    float4 g_EffectInfo;

    Transfrom g_Transform;                         // : packoffset(c0);
    matrix g_BoneTransforms[MAX_BONE_TRANSFORM_COUNT]; // : packoffset(c8);
    Material g_Material;                               // : packoffset(c520); //8 + 4 * 128
};
#else
cbuffer cbPerObject : register(b2)
{
    float2 g_ObjectID;
    float g_Offset; // �ż��� �߰�. PaperBurn �� ���� Offset��.
    float g_renderFlag;

    float4 g_EffectInfo; 

    Transfrom g_Transform; //: packoffset(c0);8
    Material g_Material;   //: packoffset(c8); 3
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
        float3 PosW : POSITION;
        float3 Tangent : TANGENT;
        float3 Normal : NORMAL;
        float2 Texcoord0 : TEXCOORD0;
        float4 matColor : Color0;
        float4 matPBR : Color1;
        uint2 objectID : TEXCOORD1;
        uint2 objectFlag : TEXCOORD2;
        uint4 Effect : Color2;
};

#ifdef GPU_INSTANCING

Buffer<float4> g_InstanceData : register(t3);

//--------------------------------------------------------------------------------------
// Vertex Shader For Instance
//--------------------------------------------------------------------------------------
VS_OUTPUT VSMain(VS_INPUT Input, uint instanceIndex : SV_InstanceID)
{
    VS_OUTPUT Output;
    int bufferStride = 0;
    float4 tilingOffset = float4(1.f, 1.f, 0.f, 0.f);

#ifdef USE_SKINNING

    bufferStride = 525;
    int bufferStartPoint = instanceIndex * bufferStride;

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

            int boneTransformStartOffset = bufferStartPoint + 10;
            int boneTrasnformOffset = boneTransformStartOffset + Input.BoneIndex[i] * 4;

            matrix boneTransformMatrix;

            boneTransformMatrix._11_12_13_14 = g_InstanceData[boneTrasnformOffset];
            boneTransformMatrix._21_22_23_24 = g_InstanceData[boneTrasnformOffset + 1];
            boneTransformMatrix._31_32_33_34 = g_InstanceData[boneTrasnformOffset + 2];
            boneTransformMatrix._41_42_43_44 = g_InstanceData[boneTrasnformOffset + 3];

            posL += Input.Weight[i] * mul(float4(Input.Position, 1.0f), boneTransformMatrix).xyz;
            normalL += Input.Weight[i] * mul(Input.Normal, (float3x3)boneTransformMatrix).xyz;
            tangentL += Input.Weight[i] * mul(Input.Tangent, (float3x3)boneTransformMatrix).xyz;
        }
    }

    posL /= totalWeight;
    normalL /= totalWeight;
    tangentL /= totalWeight;

    Output.PosH = mul(float4(posL, 1.0f), g_Camera.g_ViewProjectionMatrix);
    Output.PosW = posL;
    Output.Tangent = normalize(tangentL);
    Output.Normal = normalize(normalL);

    Output.matColor = g_InstanceData[bufferStartPoint + 522];
    Output.matPBR.x = g_InstanceData[bufferStartPoint + 523].w;
    Output.matPBR.yz = g_InstanceData[bufferStartPoint + 524].xy;
    Output.matPBR.z = 0.f;

    Output.objectID = asuint(g_InstanceData[bufferStartPoint].xy);
    Output.objectFlag = asuint(g_InstanceData[bufferStartPoint].zw);

    Output.Effect = float4(g_InstanceData[bufferStartPoint + 1].xy, 0.f, 0.f);
    tilingOffset = float4(g_InstanceData[bufferStartPoint + 525].xyzw);

#else

    bufferStride = 13;
    int bufferStartPoint = instanceIndex * bufferStride;

    matrix worldMat;

    worldMat._11_12_13_14 = g_InstanceData[bufferStartPoint + 2];
    worldMat._21_22_23_24 = g_InstanceData[bufferStartPoint + 3];
    worldMat._31_32_33_34 = g_InstanceData[bufferStartPoint + 4];
    worldMat._41_42_43_44 = g_InstanceData[bufferStartPoint + 5];

    matrix worldInvMat;

    worldInvMat._11_12_13_14 = g_InstanceData[bufferStartPoint + 6];
    worldInvMat._21_22_23_24 = g_InstanceData[bufferStartPoint + 7];
    worldInvMat._31_32_33_34 = g_InstanceData[bufferStartPoint + 8];
    worldInvMat._41_42_43_44 = g_InstanceData[bufferStartPoint + 9];

    Output.PosW = mul(float4(Input.Position, 1.0f), worldMat).xyz;
    Output.PosH = mul(float4(Output.PosW, 1.0f), g_Camera.g_ViewProjectionMatrix);
    Output.Tangent = normalize(mul(Input.Tangent, (float3x3)worldInvMat));
    Output.Normal = normalize(mul(Input.Normal, (float3x3)worldInvMat));

    Output.matColor = g_InstanceData[bufferStartPoint + 10];
    Output.matPBR.x = g_InstanceData[bufferStartPoint + 11].w;
    Output.matPBR.yz = g_InstanceData[bufferStartPoint + 12].xy;
    Output.matPBR.z = 0.f;

    Output.objectID = asuint(g_InstanceData[bufferStartPoint].xy);
    Output.objectFlag = asuint(g_InstanceData[bufferStartPoint].zw);
    Output.Effect = float4(g_InstanceData[bufferStartPoint + 1].xy, 0.f, 0.f);
    tilingOffset = float4(g_InstanceData[bufferStartPoint + 13].xyzw);

#endif
    Output.Texcoord0 = Input.Texcoord0 * tilingOffset.xy + tilingOffset.zw;

    return Output;
}

#else

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VSMain(VS_INPUT Input)
{
    VS_OUTPUT Output;
    matrix wvp = mul(g_Transform.g_World, g_Camera.g_ViewProjectionMatrix);

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
            normalL += Input.Weight[i] * mul(Input.Normal, (float3x3) g_BoneTransforms[Input.BoneIndex[i]]).xyz;
            tangentL += Input.Weight[i] * mul(Input.Tangent, (float3x3) g_BoneTransforms[Input.BoneIndex[i]]).xyz;
        }
    }

    posL /= totalWeight;
    normalL /= totalWeight;
    tangentL /= totalWeight;

    Output.PosH = mul(float4(posL, 1.0f), g_Camera.g_ViewProjectionMatrix);
    //Output.PosH = mul(float4(posL, 1.0f), wvp);
    //Output.PosW = mul(float4(posL, 1.0f), g_Transform.g_World).xyz;
    Output.PosW = posL;
    Output.Tangent = normalize(tangentL);
    Output.Normal = normalize(normalL);
#else
    Output.PosH = mul(float4(Input.Position, 1.0f), wvp);
    Output.PosW = mul(float4(Input.Position, 1.0f), g_Transform.g_World).xyz;
    Output.Tangent = normalize(mul(Input.Tangent, (float3x3) g_Transform.g_WorldInvTranspose));
    Output.Normal = normalize(mul(Input.Normal, (float3x3) g_Transform.g_WorldInvTranspose));
#endif

    Output.matColor = g_Material.Albedo;
    Output.matPBR = float4(g_Material.Metalic, g_Material.Roughness, 0.f, g_Material.Specular);
    Output.objectID = asuint(g_ObjectID);
    Output.objectFlag = uint2(asuint(g_Offset), asuint(g_renderFlag));
    Output.Effect = uint4(asuint(g_EffectInfo.x), asuint(g_EffectInfo.y), 0.f, 0.f);

    Output.Texcoord0 = Input.Texcoord0 * g_Material.Tiling.xy + g_Material.Offset.xy;

    return Output;
}
#endif 