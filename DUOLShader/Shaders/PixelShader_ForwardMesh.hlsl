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

TextureCube g_IrradianceMap : register(t3);
TextureCube g_PreFilteredMap : register(t4);
Texture2D g_BRDFLookUpTable : register(t5);
Texture2DArray g_ShadowMap : register(t6);
Texture2DArray g_SpotShadowMap : register(t7);
TextureCubeArray g_PointShadowMap : register(t8);
//TextureCubeArray g_PointShadowMap : register(t9);

SamplerState g_samLinear : register(s0);
SamplerComparisonState g_samShadow : register(s1);
SamplerState g_samAnisotropy : register(s2);
SamplerState g_samLinearClamp : register(s3);
SamplerState g_samTriLinear : register(s4);

//--------------------------------------------------------------------------------------
// Functions
//--------------------------------------------------------------------------------------
void CalculateRightAndUpTexelDepthDeltas(in float3 vShadowTexDDX,
                                         in float3 vShadowTexDDY,
                                         in float shadowResolution,
                                         out float fUpTextDepthWeight,
                                         out float fRightTextDepthWeight
 )
{

    // We use the derivatives in X and Y to create a transformation matrix.  Because these derivives give us the
    // transformation from screen space to shadow space, we need the inverse matrix to take us from shadow space
    // to screen space.  This new matrix will allow us to map shadow map texels to screen space.  This will allow
    // us to find the screen space depth of a corresponding depth pixel.
    // This is not a perfect solution as it assumes the underlying geometry of the scene is a plane.  A more
    // accureate way of finding the actual depth would be to do a deferred rendering approach and actually
    // sample the depth.

    // Using an offset, or using variance shadow maps is a better approach to reducing these artifacts in most cases.

    float2x2 matScreentoShadow = float2x2(vShadowTexDDX.xy, vShadowTexDDY.xy);
    float fDeterminant = determinant(matScreentoShadow);

    float fInvDeterminant = 1.0f / fDeterminant;

    float2x2 matShadowToScreen = float2x2(
        matScreentoShadow._22 * fInvDeterminant, matScreentoShadow._12 * -fInvDeterminant,
        matScreentoShadow._21 * -fInvDeterminant, matScreentoShadow._11 * fInvDeterminant);

    float2 vRightShadowTexelLocation = float2(shadowResolution, 0.0f);
    float2 vUpShadowTexelLocation = float2(0.0f, shadowResolution);

    // Transform the right pixel by the shadow space to screen space matrix.
    float2 vRightTexelDepthRatio = mul(vRightShadowTexelLocation, matShadowToScreen);
    float2 vUpTexelDepthRatio = mul(vUpShadowTexelLocation, matShadowToScreen);

    // We can now caculate how much depth changes when you move up or right in the shadow map.
    // We use the ratio of change in x and y times the dervivite in X and Y of the screen space
    // depth to calculate this change.
    fUpTextDepthWeight =
        vUpTexelDepthRatio.x * vShadowTexDDX.z 
        + vUpTexelDepthRatio.y * vShadowTexDDY.z;
    fRightTextDepthWeight =
        vRightTexelDepthRatio.x * vShadowTexDDX.z 
        + vRightTexelDepthRatio.y * vShadowTexDDY.z;
}

float CalcShadowFactor(in SamplerComparisonState samShadow, in Texture2DArray shadowMap, in float3 posW, in float shadowResolution, inout int cascadeSlice)
{

    const float dx = 1.f / (shadowResolution);
    const int CASCADE_MAX_COUNT = 4;

    float viewpos = mul(float4(posW, 1.f), g_Camera.g_ViewMatrix).z;
    viewpos /= (g_Camera.g_CameraFar - g_Camera.g_CameraNear);

    for (cascadeSlice = 0; cascadeSlice < 4; cascadeSlice++)
    {
        if (viewpos < g_shadow.CascadeOffset[cascadeSlice])
        {
            break;
        }
    }

    float4 lightSpacePos = mul(float4(posW.xyz, 1.f), g_shadow.Shadow[cascadeSlice]);
    lightSpacePos /= lightSpacePos.w;

    float percentLit = 0.0f;
    float percentLit_blend = 0.0f;

    // Variance 해보자
    const float2 offsets[9] =
    {
        float2(-dx, -dx), float2(0.f, -dx), float2(+dx, -dx),
            float2(-dx, 0.f), float2(0.f, 0.f), float2(+dx, 0.f),
            float2(-dx, +dx), float2(0.f, +dx), float2(+dx, +dx)
    };

    float2 uv = ViewPosToUV(lightSpacePos.xy);

    float3 vShadowMapTextureCoordDDX;
    float3 vShadowMapTextureCoordDDY;

    for (int i = 0; i < 9; i++)
    {
        percentLit += shadowMap.SampleCmpLevelZero(samShadow, float3(uv + offsets[i], cascadeSlice), lightSpacePos.z).r;
    }
    percentLit /= 9.0f;

    // int blendCascadeSlice = min(cascadeSlice + 1, CASCADE_MAX_COUNT);

    // if (cascadeSlice < CASCADE_MAX_COUNT && blendCascadeSlice > 0)
    // {
    //     float delta = abs(g_shadow.CascadeOffset[cascadeSlice] - lightSpacePos.z);
    //     if (delta < 0.07)
    //     {
    //     float3 lightSpacePos_blend = mul(float4(posW.xyz, 1.f), g_shadow.Shadow[blendCascadeSlice]).xyz;

    //     float2 uv_blend = ViewPosToUV(lightSpacePos_blend.xy);

    //     // blend한다.
    //     for (int i = 0; i < 9; i++)
    //     {
    //         percentLit_blend += shadowMap.SampleCmpLevelZero(samShadow, float3(uv_blend + offsets[i], blendCascadeSlice), lightSpacePos_blend.z).r;
    //     }
    //     percentLit_blend /= 9.0f;

    //     // calcBlendAmount
    //     int previousSlice = cascadeSlice - 1;
    //     float startPoint = 0.0f;
    //     if (previousSlice > -1)
    //     {
    //         startPoint = g_shadow.CascadeOffset[previousSlice];
    //     }

    //     float pos = viewpos - startPoint;
    //     float interval = g_shadow.CascadeOffset[blendCascadeSlice] - startPoint;
    //     float blendAmount = pos / interval;

    //     percentLit = lerp(percentLit, percentLit_blend, blendAmount);
    //     }
    // }

    // for light bleeding
    static const float magicNumber = 2.f;
    percentLit = pow(percentLit, magicNumber);

    return percentLit;
}

float CalcShadowFactorFromSpotShadowMap(in SamplerComparisonState samShadow, in Texture2DArray shadowMap, in float3 posW, in float shadowResolution, in int lightIdx)
{
    const float dx = 1.f / (shadowResolution);

    float4 lightSpacePos = mul(float4(posW.xyz, 1.f), g_Light[lightIdx].ShadowMatrix[0]);
    lightSpacePos /= lightSpacePos.w;

    float percentLit = 0.0f;
    float percentLit_blend = 0.0f;

    // Variance 해보자
    const float2 offsets[9] =
    {
        float2(-dx, -dx), float2(0.f, -dx), float2(+dx, -dx),
            float2(-dx, 0.f), float2(0.f, 0.f), float2(+dx, 0.f),
            float2(-dx, +dx), float2(0.f, +dx), float2(+dx, +dx)
    };

    float2 uv = ViewPosToUV(lightSpacePos.xy);

    float3 vShadowMapTextureCoordDDX;
    float3 vShadowMapTextureCoordDDY;

    switch (g_Light[lightIdx].ShadowState)
    {
        case 1:
    {
                for (int i = 0; i < 9; i++)
                {
                    float staticValue = shadowMap.SampleCmpLevelZero(samShadow, float3(uv + offsets[i], g_Light[lightIdx].staticShadowMapIdx), lightSpacePos.z).r;
                    percentLit += staticValue;
                }
                break;
            }
        case 2:
    {
                for (int i = 0; i < 9; i++)
                {

                    float dynamicValue = shadowMap.SampleCmpLevelZero(samShadow, float3(uv + offsets[i], g_Light[lightIdx].dynamicShadowMapIdx), lightSpacePos.z).r;
                    float staticValue = shadowMap.SampleCmpLevelZero(samShadow, float3(uv + offsets[i], g_Light[lightIdx].staticShadowMapIdx), lightSpacePos.z).r;
                    percentLit += max(dynamicValue, staticValue);
                }
                break;
            }
        case 3:
    {
                for (int i = 0; i < 9; i++)
                {

                    float dynamicValue = shadowMap.SampleCmpLevelZero(samShadow, float3(uv + offsets[i], g_Light[lightIdx].dynamicShadowMapIdx), lightSpacePos.z).r;
                    percentLit += dynamicValue;
                }
                break;
            }
        default:
            break;
    }

    percentLit /= 9.0f;

    return percentLit;
}

// z값 뽑아내기
float CalcDepthInShadow(const in float3 fragPos, float far_plane, float near_plane)
{
    const float c1 = far_plane / (far_plane - near_plane);
    const float c0 = -near_plane * far_plane / (far_plane - near_plane);
    const float3 m = abs(fragPos).xyz;
    const float major = max(m.x, max(m.y, m.z));
    return (c1 * major + c0) / major;
}

float CalcShadowFactorFromPointShadowMap(in SamplerComparisonState samShadow, in TextureCubeArray shadowMap, in float3 posW, in float shadowResolution, in int lightIdx)
{
    const float dx = 1.f / (shadowResolution);

    float3 uv = posW - g_Light[lightIdx].Position;
    float depth = CalcDepthInShadow(uv, g_Light[lightIdx].AttenuationRadius, 0.1f);
    uv = normalize(uv);

    float percentLit = 0.0f;
    float percentLit_blend = 0.0f;

    const float3 sampleOffsetDirections[21] =
    {
        float3(dx, dx, dx), float3(dx, -dx, dx), float3(-dx, -dx, dx), float3(-dx, dx, dx),
          float3(dx, dx, -dx), float3(dx, -dx, -dx), float3(-dx, -dx, -dx), float3(-dx, dx, -dx),
          float3(dx, dx, 0), float3(dx, -dx, 0), float3(-dx, -dx, 0), float3(-dx, dx, 0),
          float3(dx, 0, dx), float3(-dx, 0, dx), float3(dx, 0, -dx), float3(-dx, 0, -dx),
          float3(0, dx, dx), float3(0, -dx, dx), float3(0, -dx, -dx), float3(0, dx, -dx), float3(0, 0, 0)
    };

    for (int i = 0; i < 21; i++)
    {
        // float dynamicValue = shadowMap.SampleLevel(g_samLinear, float4(uv, g_Light[lightIdx].dynamicShadowMapIdx), depth).r;
        float dynamicValue = shadowMap.SampleLevel(g_samLinear, float4(uv + sampleOffsetDirections[i], g_Light[lightIdx].dynamicShadowMapIdx), 0).r;
        percentLit += (dynamicValue >= depth) ? 1 : 0;
    }

    percentLit /= 21.0f;

    return percentLit;
}

float CalcShadowFactorFromAreaShadowMap(in SamplerComparisonState samShadow, in Texture2DArray shadowMap, in float3 posW, in float shadowResolution, in int lightIdx)
{
    const float dx = 1.f / (shadowResolution);

    float4 lightSpacePos = mul(float4(posW.xyz, 1.f), g_Light[lightIdx].ShadowMatrix[0]);
    lightSpacePos /= lightSpacePos.w;

    float percentLit = 0.0f;
    float percentLit_blend = 0.0f;

    // Variance 해보자
    const float2 offsets[9] =
    {
        float2(-dx, -dx), float2(0.f, -dx), float2(+dx, -dx),
            float2(-dx, 0.f), float2(0.f, 0.f), float2(+dx, 0.f),
            float2(-dx, +dx), float2(0.f, +dx), float2(+dx, +dx)
    };

    float2 uv = ViewPosToUV(lightSpacePos.xy);

    float3 vShadowMapTextureCoordDDX;
    float3 vShadowMapTextureCoordDDY;

    switch (g_Light[lightIdx].ShadowState)
    {
        case 1:
    {
                for (int i = 0; i < 9; i++)
                {
                    float staticValue = shadowMap.SampleCmpLevelZero(samShadow, float3(uv + offsets[i], g_Light[lightIdx].staticShadowMapIdx), lightSpacePos.z).r;
                    percentLit += staticValue;
                }
                break;
            }
        case 2:
    {
                for (int i = 0; i < 9; i++)
                {

                    float dynamicValue = shadowMap.SampleCmpLevelZero(samShadow, float3(uv + offsets[i], g_Light[lightIdx].dynamicShadowMapIdx), lightSpacePos.z).r;
                    float staticValue = shadowMap.SampleCmpLevelZero(samShadow, float3(uv + offsets[i], g_Light[lightIdx].staticShadowMapIdx), lightSpacePos.z).r;
                    percentLit += max(dynamicValue, staticValue);
                }
                break;
            }
        case 3:
    {
                for (int i = 0; i < 9; i++)
                {

                    float dynamicValue = shadowMap.SampleCmpLevelZero(samShadow, float3(uv + offsets[i], g_Light[lightIdx].dynamicShadowMapIdx), lightSpacePos.z).r;
                    percentLit += dynamicValue;
                }
                break;
            }
        default:
            break;
    }

    percentLit /= 9.0f;

    return percentLit;
}

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
struct OITOut
{
    float4 OverColor : SV_Target0;
    float4 OverInfo : SV_Target1;
    float4 AdditiveColor : SV_Target2;
    float4 AdditiveInfo : SV_Target3;
};

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
[earlydepthstencil]
OITOut PSMain(PS_INPUT Input)
{
    OITOut oitOut;
    
    oitOut.OverColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
    oitOut.OverInfo = float4(0.0f, 0.0f, 0.0f, 1.0f);
    oitOut.AdditiveColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
    oitOut.AdditiveInfo = float4(0.0f, 0.0f, 0.0f, 1.0f);

    ObjectFactors objectFactor;

#ifdef USING_ALBEDO
    objectFactor.albedo = g_DiffuseTexture.Sample(g_samLinear, Input.Texcoord0);
    // gamma correction
    objectFactor.albedo.xyz = pow(objectFactor.albedo.xyz, 2.0f);
#else
    objectFactor.albedo = float4(Input.matColor.xyzw);
#endif
    
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
    objectFactor.metallicRoughnessAOSpecular = float4(Input.matPBR.x, Input.matPBR.y, 1.0f, Input.matPBR.z);
#endif

    objectFactor.posW = float4(Input.PosW, 1.0f);
    // // Wvalue is Flag
    // psOut.ObjectID = float4(asfloat(Input.objectID.xy), asfloat(Input.objectFlag.xy));
    // psOut.Effect = asfloat(Input.Effect);
    //return psOut;

    float metallic = objectFactor.metallicRoughnessAOSpecular.x;
    float roughness = max(0.001f, objectFactor.metallicRoughnessAOSpecular.y);

    static const float kSpecularCoefficient = 0.08;

    // blend base Color Width Metallic
    // 메탈릭 수치에 따라
    float specular = kSpecularCoefficient * objectFactor.metallicRoughnessAOSpecular.w;

    const float3 c_diff = lerp(objectFactor.albedo.xyz, float3(0, 0, 0), metallic);
    const float3 c_spec = lerp((float3) specular, objectFactor.albedo.xyz, metallic);
   
    float3 eyeVec = normalize(g_Camera.g_CameraPosition.xyz - objectFactor.posW.xyz);

    // outputColor
    float4 Output;
    Output = float4(0.f, 0.f, 0.f, 0.f);

    [unroll]
    for (uint lightIdx = 0; lightIdx < g_LightCnt; lightIdx++)
    {
        if (g_Light[lightIdx].Type == Directional)
        {
            float shadows = 1.0f;

            float clipSpaceDepth = objectFactor.normal.w;
            int cascadeSlice = 0;
            // shadow
            // 2048 -> hard code shader map size
            shadows *= CalcShadowFactor(g_samShadow, g_ShadowMap, objectFactor.posW.xyz, 2048.f, cascadeSlice);
            // shadows = 1.0f;

            Output += ComputePBRDirectionalLight(g_Light[lightIdx], c_spec, c_diff, objectFactor.normal.xyz, eyeVec, roughness, metallic) * shadows;
        }
        else if (g_Light[lightIdx].Type == Point)
        {
            float shadows = 1.0f;
            int idx = asint(lightIdx);

            shadows *= CalcShadowFactorFromPointShadowMap(g_samShadow, g_PointShadowMap, objectFactor.posW.xyz, 1024.f, idx);
            Output += ComputePBRPointLight(g_Light[lightIdx], c_spec, c_diff, objectFactor.normal.xyz, eyeVec, roughness, metallic, objectFactor.posW.xyz) * shadows;
            // shadows = 1.0f;
        }
        else if (g_Light[lightIdx].Type == Spot)
        {
            float shadows = 1.0f;
            int idx = asint(lightIdx);
            // shadow
            // 2048 -> hard code shader map size
            shadows *= CalcShadowFactorFromSpotShadowMap(g_samShadow, g_SpotShadowMap, objectFactor.posW.xyz, 1024.f, idx);
            // shadows = 1.0f;

            Output += ComputePBRSpotLight(g_Light[lightIdx], c_spec, c_diff, objectFactor.normal.xyz, eyeVec, roughness, metallic, objectFactor.posW.xyz) * shadows;
        }
        else if (g_Light[lightIdx].Type == AreaRect)
        {
            float shadows = 1.0f;

            shadows *= CalcShadowFactorFromAreaShadowMap(g_samShadow, g_SpotShadowMap, objectFactor.posW.xyz, 1024.f, lightIdx);
            Output += ComputePBRAreaRectLight(g_Light[lightIdx], c_spec, c_diff, objectFactor.normal.xyz, eyeVec, roughness, metallic, objectFactor.posW.xyz) * shadows;
        }
        else
        {
        }
    }

    float3 diffuseIrradiance = g_IrradianceMap.Sample(g_samLinear, objectFactor.normal.xyz).xyz;

    // Diffuse IBL
    Output.xyz += c_diff * diffuseIrradiance;

    // Specular IBL
    const float MAX_REFLECTION_LOD = 4.0; // 0~3, 4개의 밉맵 존재합니다.
    float mipLevel = roughness * MAX_REFLECTION_LOD;
    float3 R = reflect(-eyeVec, objectFactor.normal.xyz);

    float3 prefilteredColor = g_PreFilteredMap.SampleLevel(g_samTriLinear, normalize(R).xyz, mipLevel).rgb;
    float NdotV = dot(objectFactor.normal.xyz, eyeVec);
    float2 BRDF = g_BRDFLookUpTable.Sample(g_samLinearClamp, float2(max(NdotV, 0.f), roughness)).rg;

    float3 specularIBL = prefilteredColor * (c_spec * BRDF.x + BRDF.y);
    Output.xyz += specularIBL;

    Output.w = objectFactor.albedo.w;

    float4 color = Output;

    if (color.a <= 0.000001f)
        clip(-1.0f);

    float z = Input.PosH.z * Input.PosH.w;
    float w = color.w * max(0.01f, min(3000.0f, 0.03f / (0.00001f + pow(abs(z) / 200.0f, 4.0f))));
        
    oitOut.OverColor = float4((color.xyz) * color.a, color.a) * w;
    oitOut.OverInfo.x = Input.PosH.z * Input.PosH.w;
    oitOut.OverInfo.y = 1.0f;
    oitOut.OverInfo.z = color.a * w;
    oitOut.OverInfo.a = 1 - color.a;

    return oitOut;

}