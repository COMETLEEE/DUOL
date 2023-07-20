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
Texture2D g_EmissiveTexture : register(t3);

Texture2D g_NoiseTexture : register(t4);

SamplerState g_samLinear : register(s0);

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
    float4 matEmissive : Color2;
    uint2 objectID : TEXCOORD1;
    uint2 objectFlag : TEXCOORD2;
    uint4 Effect : Color3;
};

struct PS_OUT
{
    float4 Albedo : SV_Target0;
    float4 Normal : SV_Target1;
    float4 World : SV_Target2;
    float4 MetalicRoughnessAOSpecular : SV_Target3; // 4byte pack 256 256 256 256
    float4 ObjectID : SV_Target4; // 8byte
    float4 Effect : SV_Target5; // 16byte
};

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
PS_OUT PSMain(PS_INPUT Input) : SV_TARGET
{
    PS_OUT psOut;

#ifdef USING_ALBEDO
	psOut.Albedo = g_DiffuseTexture.Sample(g_samLinear, Input.Texcoord0);
    // gamma correction
    psOut.Albedo.xyz = pow(psOut.Albedo.xyz, 2.0f);
#else
#endif
    psOut.Albedo = float4(Input.matColor.xyz, 1.f);
    //alpha Clipping 하드코딩..
    if (psOut.Albedo.w < 0.5f)
    {
        clip(-1);
    }
    
float noise = g_NoiseTexture.Sample(g_samLinear, float2(Input.Texcoord0.x, g_Offset)).x;
float4 paperBurnColor1 = float4(1.f,0.f,0.f,1.f);

float4 posW = float4(Input.PosW, 1.f);
float4 posL = mul(posW, transpose(g_Transform.g_WorldInvTranspose));

noise /= 5.f;
//g_Offset은 메쉬의 local, 가장 낮은 좌표부터 올라간다.

if (posL.y > g_Offset + noise)
{
    clip(-1);
}
else if (g_Offset + noise - posL.y < 0.2f)
{
    psOut.Albedo = lerp(paperBurnColor1 * 10.0f, psOut.Albedo, clamp((g_Offset + noise - posL.y) * 5.0f, 0.0f , 1.0f));
}

uint packedemissive = 0;
float4 emissive;

#ifdef USING_EMISSIVE
emissive = g_EmissiveTexture.Sample(g_samLinear, Input.Texcoord0);
#else
emissive = float4(Input.matEmissive.xyz, 0.f);
#endif

emissive.w = Input.matEmissive.w;
packedemissive = PackingColor(emissive);
// wValue is Emissive Value


#ifdef USING_NORMALMAP
   float3 normalMapSample = g_NormalMapTexture.Sample(g_samLinear, Input.Texcoord0).rgb;
   psOut.Normal.xyz = pow(psOut.Normal.xyz, 2.0f);

   float3 normalT = normalMapSample * 2.0f - 1.0f;

   float3 N = Input.Normal;
   float3 T = normalize(Input.Tangent - dot(Input.Tangent, N) * N);
   float3 B = cross(N, T);

   float3x3 tanspaceMat = float3x3(T, B, N);

   psOut.Normal = float4(normalize(mul(normalT, tanspaceMat)), asfloat(packedemissive));
#else
    psOut.Normal = (float4(normalize(Input.Normal), Input.PosH.z));
#endif

#ifdef USING_METALICROUGHNESS
   psOut.MetalicRoughnessAOSpecular = g_MetalicRoughnessTexture.Sample(g_samLinear, Input.Texcoord0);
   // psOut.MetalicRoughnessAOSpecular.w = Input.matPBR.z;
   psOut.MetalicRoughnessAOSpecular.w = 0.5f;
   //psOut.MetalicRoughnessAOSpecular.w = 1.f;

    // gamma correction
    psOut.MetalicRoughnessAOSpecular.xyz = pow(psOut.MetalicRoughnessAOSpecular.xyz, 2.0f);
#else
    //psOut.MetalicRoughnessAOSpecular = float4(Input.matPBR.x, Input.matPBR.y, 0.0f, Input.matPBR.z);
    psOut.MetalicRoughnessAOSpecular = float4(Input.matPBR.x, Input.matPBR.y, 1.0f, 0.5f);
#endif
    psOut.MetalicRoughnessAOSpecular = float4(Input.matPBR.x, Input.matPBR.y, 1.0f, 0.5f);




    psOut.World = float4(Input.PosW, Input.PosH.z);
    // Wvalue is Flag
    psOut.ObjectID = float4(asfloat(Input.objectID.xy), asfloat(Input.objectFlag.xy));
    psOut.Effect = asfloat(Input.Effect);

    return psOut;
}