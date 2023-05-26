#include "ConstantBuffer.hlsli"

#define MAX_BONE_TRANSFORM_COUNT 128
//--------------------------------------------------------------------------------------
// Globals
//--------------------------------------------------------------------------------------
//// PerObject


#ifdef USE_SKINNING
cbuffer cbPerObject : register(b2)
{
    float2 g_ObjectID;
    float g_Offset; // �ż��� �߰�. PaperBurn �� ���� Offset��.
    float g_renderFlag;

    float4 g_EffectInfo;

    Transfrom g_Transform; // : packoffset(c0);
    matrix g_BoneTransforms[MAX_BONE_TRANSFORM_COUNT]; // : packoffset(c8);
    Material g_Material; // : packoffset(c520); //8 + 4 * 128
};
#else
cbuffer cbPerObject : register(b2)
{
    float2 g_ObjectID;
    float g_Offset; // �ż��� �߰�. PaperBurn �� ���� Offset��.
    float g_renderFlag;

    float4 g_EffectInfo;

    Transfrom g_Transform; //: packoffset(c0);
    Material g_Material; //: packoffset(c8);
}
#endif
//--------------------------------------------------------------------------------------
// Textures and Samplers
//--------------------------------------------------------------------------------------

Texture2D g_DiffuseTexture : register(t0);
Texture2D g_NormalMapTexture : register(t1);
Texture2D g_MetalicRoughnessTexture : register(t2);

SamplerState g_samLinear : register(s0);
SamplerComparisonState g_samShadow : register(s1);
SamplerState g_samAnisotropy : register(s2);
SamplerState g_samLinearClamp : register(s3);
SamplerState g_samTriLinear : register(s4);

//--------------------------------------------------------------------------------------
// Functions
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Input / Output structures
//--------------------------------------------------------------------------------------

struct PS_INPUT
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

struct ObjectFactors
{
    float4 albedo;
    float4 normal;
    float4 metallicRoughnessAOSpecular;
    float4 posW;
};

struct DepthOut
{
    float4 Normal : SV_Target0;
    float4 World : SV_Target1;
    float4 MetalicRoughnessAOSpecular : SV_Target2; // 4byte pack 256 256 256 256
    float4 ObjectID : SV_Target3;                   // 8byte
    float4 Effect : SV_Target4;
};
//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
[earlydepthstencil]
DepthOut PSMain(PS_INPUT Input)
{
    DepthOut depthOut;

    ObjectFactors objectFactor;
   
#ifdef USING_NORMALMAP
    float3 normalMapSample = g_NormalMapTexture.Sample(g_samLinear, Input.Texcoord0).rgb;
    objectFactor.normal.xyz = pow(objectFactor.normal.xyz, 2.0f);

   float3 normalT = normalMapSample * 2.0f - 1.0f;

   float3 N = Input.Normal;
   float3 T = normalize(Input.Tangent - dot(Input.Tangent, N) * N);
   float3 B = cross(N, T);

   float3x3 tanspaceMat = float3x3(T, B, N);

   objectFactor.normal = float4(normalize(mul(normalT, tanspaceMat)), Input.PosH.z);
#else
    objectFactor.normal = (float4(normalize(Input.Normal), Input.PosH.z));
#endif

#ifdef USING_METALICROUGHNESS
    objectFactor.metallicRoughnessAOSpecular = g_MetalicRoughnessTexture.Sample(g_samLinear, Input.Texcoord0);
    objectFactor.metallicRoughnessAOSpecular.w = Input.matPBR.z;
   //psOut.MetalicRoughnessAOSpecular.w = 1.f;

   // gamma correction
   objectFactor.metallicRoughnessAOSpecular.xyz = pow(objectFactor.metallicRoughnessAOSpecular, 2.0f);
#else
    objectFactor.metallicRoughnessAOSpecular = float4(Input.matPBR.x, Input.matPBR.y, 0.0f, Input.matPBR.z);
#endif

    objectFactor.posW = float4(Input.PosW, 1.0f);

    depthOut.Normal = objectFactor.normal;
    depthOut.World = float4(Input.PosW, 1.f);
    depthOut.MetalicRoughnessAOSpecular = objectFactor.metallicRoughnessAOSpecular;
    depthOut.ObjectID = float4(asfloat(Input.objectID.xy), asfloat(Input.objectFlag.xy));
    depthOut.Effect = asfloat(Input.Effect);

    return depthOut;

}