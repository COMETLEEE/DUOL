#include "ConstantBuffer.hlsli"

//--------------------------------------------------------------------------------------
// Globals
//--------------------------------------------------------------------------------------
//// PerObject
cbuffer cbPerObject : register(b2) {};
//--------------------------------------------------------------------------------------
// Textures and Samplers
//--------------------------------------------------------------------------------------

Texture2D g_Albedo : register(t0);
Texture2D g_Normal : register(t1);
Texture2D g_PosW : register(t2);
Texture2D g_MetallicRoughnessAOSpecular : register(t3);
TextureCube g_IrradianceMap : register(t4);
TextureCube g_PreFilteredMap : register(t5);
Texture2D g_BRDFLookUpTable : register(t6);
Texture2DArray g_ShadowMap : register(t7);
Texture2DArray g_SpotShadowMap : register(t8);
TextureCubeArray g_PointShadowMap : register(t9);

SamplerState g_samLinear : register(s0);
SamplerComparisonState g_samShadow : register(s1);
SamplerState g_samAnisotropy : register(s2);
SamplerState g_samLinearClamp : register(s3);
SamplerState g_samTriLinear : register(s4);


//--------------------------------------------------------------------------------------
// Input / Output structures
//--------------------------------------------------------------------------------------
struct PS_INPUT
{
    float4 Position : SV_POSITION;
    float2 Texcoord0 : TEXCOORD0;
};

//--------------------------------------------------------------------------------------
// Functions
//--------------------------------------------------------------------------------------
void CalculateRightAndUpTexelDepthDeltas(in float3 vShadowTexDDX,
                                         in float3 vShadowTexDDY,
                                         in float shadowResolution,
                                         out float fUpTextDepthWeight,
                                         out float fRightTextDepthWeight
 ) {

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

//z값 뽑아내기
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
        { float3(dx, dx, dx), float3(dx, -dx, dx), float3(-dx, -dx, dx), float3(-dx, dx, dx),
          float3(dx, dx, -dx), float3(dx, -dx, -dx), float3(-dx, -dx, -dx), float3(-dx, dx, -dx),
          float3(dx, dx, 0), float3(dx, -dx, 0), float3(-dx, -dx, 0), float3(-dx, dx, 0),
          float3(dx, 0, dx), float3(-dx, 0, dx), float3(dx, 0, -dx), float3(-dx, 0, -dx),
          float3(0, dx, dx), float3(0, -dx, dx), float3(0, -dx, -dx), float3(0, dx, -dx), float3(0, 0, 0) };



    for (int i = 0; i < 21; i++)
    {
            // float dynamicValue = shadowMap.SampleLevel(g_samLinear, float4(uv, g_Light[lightIdx].dynamicShadowMapIdx), depth).r;
            float dynamicValue = shadowMap.SampleLevel(g_samLinear, float4(uv + sampleOffsetDirections[i], g_Light[lightIdx].dynamicShadowMapIdx), 0).r;
            percentLit += (dynamicValue >= depth)? 1: 0; 
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
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PSMain(PS_INPUT Input)
    : SV_Target
{
    // outputColor
    float4 Output;

    Output = float4(0.f, 0.f, 0.f, 0.f);

    float4 albedo = g_Albedo.Load(int3(Input.Position.xy, 0));
    if (albedo.w == 0.f)
        clip(-1);

    float4 normal = g_Normal.Load(int3(Input.Position.xy, 0));
    float4 posW = g_PosW.Load(int3(Input.Position.xy, 0));
    float4 metallicRoughnessAOSpecular = g_MetallicRoughnessAOSpecular.Load(int3(Input.Position.xy, 0));
    float3 diffuseIrradiance = g_IrradianceMap.Sample(g_samLinear, normal.xyz).xyz;
    float3 eyeVec = normalize(g_Camera.g_CameraPosition.xyz - posW.xyz);
    float distance = length(g_Camera.g_CameraPosition.xyz - posW.xyz);

    float metallic = metallicRoughnessAOSpecular.x;
    float roughness = max(0.001f, metallicRoughnessAOSpecular.y);

    static const float kSpecularCoefficient = 0.08;

    // blend base Color Width Metallic
    // 메탈릭 수치에 따라
    float specular = kSpecularCoefficient * metallicRoughnessAOSpecular.w;

    const float3 c_diff = lerp(albedo.xyz, float3(0, 0, 0), metallic);
    const float3 c_spec = lerp((float3)specular, albedo.xyz, metallic);

    [unroll]
    for (uint lightIdx = 0; lightIdx < g_LightCnt; lightIdx++)
    {
            if (g_Light[lightIdx].Type == Directional)
        {
            float shadows = 1.0f;

            float clipSpaceDepth = normal.w;
            int cascadeSlice = 0;
            // shadow
            // 2048 -> hard code shader map size
            shadows *= CalcShadowFactor(g_samShadow, g_ShadowMap, posW.xyz, 2048.f, cascadeSlice);
            // shadows = 1.0f;

            Output += ComputePBRDirectionalLight(g_Light[lightIdx], c_spec, c_diff, normal.xyz, eyeVec, roughness, metallic) * shadows;
        }
        else if (g_Light[lightIdx].Type == Point)
        {
            float shadows = 1.0f;
            int idx = asint(lightIdx);

            shadows *= CalcShadowFactorFromPointShadowMap(g_samShadow, g_PointShadowMap, posW.xyz, 1024.f, idx);
            Output += ComputePBRPointLight(g_Light[lightIdx], c_spec, c_diff, normal.xyz, eyeVec, roughness, metallic, posW.xyz) * shadows;
            // shadows = 1.0f;
        }
        else if (g_Light[lightIdx].Type  == Spot)
        {
            float shadows = 1.0f;
            int idx = asint(lightIdx);
            // shadow
            // 2048 -> hard code shader map size
            shadows *= CalcShadowFactorFromSpotShadowMap(g_samShadow, g_SpotShadowMap, posW.xyz, 1024.f, idx);
            // shadows = 1.0f;

            Output += ComputePBRSpotLight(g_Light[lightIdx], c_spec, c_diff, normal.xyz, eyeVec, roughness, metallic, posW.xyz) * shadows ;
        }
        else if (g_Light[lightIdx].Type == AreaRect)
        {
            float shadows = 1.0f;

            shadows *= CalcShadowFactorFromAreaShadowMap(g_samShadow, g_SpotShadowMap, posW.xyz, 1024.f, lightIdx);
            Output += ComputePBRAreaRectLight(g_Light[lightIdx], c_spec, c_diff, normal.xyz, eyeVec, roughness, metallic, posW.xyz) * shadows;

        }
        else
        {
        }
    }

    //Diffuse IBL
    Output.xyz += c_diff * diffuseIrradiance;

    //Specular IBL
    const float MAX_REFLECTION_LOD = 4.0; // 0~3, 4개의 밉맵 존재합니다.
    float mipLevel = roughness * MAX_REFLECTION_LOD;
    float3 R = reflect(-eyeVec, normal.xyz);

    float3 prefilteredColor = g_PreFilteredMap.SampleLevel(g_samTriLinear, normalize(R).xyz, mipLevel).rgb;
    float NdotV = dot(normal.xyz, eyeVec);
    float2 BRDF = g_BRDFLookUpTable.Sample(g_samLinearClamp, float2(max(NdotV, 0.f), roughness)).rg;

    float3 specularIBL = prefilteredColor * (c_spec * BRDF.x + BRDF.y);
    Output.xyz += specularIBL;
  
    //Set Alpha
    Output.w = albedo.w;

    //cascade debug
    // if (cascadeSlice == 0)
    // {
    //     Output.r += 0.2f;
    // }
    // else if (cascadeSlice == 1)
    // {
    //     Output.g += 0.2f;
    // }
    // else if (cascadeSlice == 2)
    // {
    //     Output.b += 0.2f;
    // }
    // else
    // {
    //     Output.r += 0.5f;
    //     Output.g += 0.5f;
    // }

    return Output;
}
