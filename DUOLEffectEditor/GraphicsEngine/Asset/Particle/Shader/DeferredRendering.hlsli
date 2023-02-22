#include "ConstantBuffer.hlsli"

#define NUM_MOTIONBLUR_SAMPLES     8

Texture2D gAlbedoBuffer : register(t0); // 출력하기 위함
Texture2D gDepthBuffer : register(t1); // 디퍼드 조립을 위한 버퍼들.
Texture2D gNormalBuffer : register(t2);
Texture2D gPositionBuffer : register(t3);
Texture2D gMetallicRoughnessAOSpecularBuffer : register(t4);
TextureCube gDiffuseRadianceBuffer : register(t5);
TextureCube gPreFilterMapBuffer : register(t6);
Texture2D gBRDFLookUpTableBuffer : register(t7);

SamplerState g_samLinear : register(s1);
SamplerState g_samLinearClamp : register(s2);
struct VS_IN
{
    float3 pos : POSITION;
    float2 uv : UV;
};

struct VS_OUT
{
    float4 pos : SV_POSITION;
    float2 uv : UV;
};

// 버텍스 셰이더의 본질인 함수
VS_OUT VS_MAIN(VS_IN input)
{
    VS_OUT output;

    output.pos = float4(input.pos, 1.0f);
    output.uv = input.uv;
    return output;
}

// 텍스처 출력을 하는 쉐이더
float4 PS_TextureRender(VS_OUT input) : SV_Target
{
    return gAlbedoBuffer.Sample(samAnisotropic, input.uv);
}

// 포스트 프로세싱
float4 PS_PostProcess(VS_OUT input) : SV_Target
{
    float4 color = gAlbedoBuffer.Sample(samAnisotropic, input.uv);

    return color;
}

// 디퍼드 조립.
float4 PS_DeferredRender(VS_OUT input) : SV_Target
{
    // outputColor
    float4 Output;

    Output = float4(0.f, 0.f, 0.f, 0.f);

    float4 albedo = gAlbedoBuffer.Sample(g_samLinear, input.uv);
    
    return albedo;
    // float4 albedo = gAlbedoBuffer.Sample(samAnisotropic, input.uv);
    
    clip(albedo.w - 0.00001f);
    
    float4 normal = gNormalBuffer.Sample(g_samLinear, input.uv);
    
    // 큐브맵에 라이팅을 안하기 위해서.. 나중에 바꾸자..
    if (length(normal) <= 0.00001f)
        return albedo;
    // 큐브맵에 라이팅을 안하기 위해서.. 나중에 바꾸자..
        
    float4 posW = gPositionBuffer.Sample(g_samLinear, input.uv);
    float4 metallicRoughnessAOSpecular = gMetallicRoughnessAOSpecularBuffer.Sample(g_samLinear, input.uv);
    float3 diffuseRadiance = gDiffuseRadianceBuffer.Sample(g_samLinear, normal.xyz).xyz;
    float3 eyeVec = normalize(gCameraPosW - posW.xyz);
    float distance = length(gCameraPosW - posW.xyz);

    static const float kSpecularCoefficient = 0.08;

    // blend base Color Width Metallic
    // 메탈릭 수치에 따라
    float specular = kSpecularCoefficient * metallicRoughnessAOSpecular.w;
    const float3 c_diff = lerp(albedo.xyz, float3(0, 0, 0), metallicRoughnessAOSpecular.x);
    const float3 c_spec = lerp((float3) specular, albedo.xyz, metallicRoughnessAOSpecular.x);


    [unroll]
    for (uint lightIdx = 0; lightIdx < gLightCount; lightIdx++)
    {
        if (gLight[lightIdx].Type < 2)
        {
            Output += ComputePBRDirectionalLight(gLight[lightIdx], c_spec, c_diff, normal.xyz, eyeVec, metallicRoughnessAOSpecular.y, metallicRoughnessAOSpecular.x);
        }
        else if (gLight[lightIdx].Type < 3)
        {
            Output += ComputePBRPointLight(gLight[lightIdx], c_spec, c_diff, normal.xyz, eyeVec, metallicRoughnessAOSpecular.y, metallicRoughnessAOSpecular.x, posW.xyz);
        }
        else if (gLight[lightIdx].Type < 4)
        {
            Output += ComputePBRSpotLight(gLight[lightIdx], c_spec, c_diff, normal.xyz, eyeVec, metallicRoughnessAOSpecular.y, metallicRoughnessAOSpecular.x, posW.xyz);
        }
    }

    // Diffuse IBL
    Output = float4(Output.xyz, albedo.w);
    Output.xyz += c_diff * diffuseRadiance;
    
    // Specular IBL
    const float MAX_REFLECTION_LOD = 4.0; // 0~4개의 밉맵 존재합니다.
    float mipLevel = metallicRoughnessAOSpecular.y * MAX_REFLECTION_LOD;
    float3 R = reflect(-eyeVec, normal.xyz);

    float3 prefilteredColor = gPreFilterMapBuffer.SampleLevel(g_samLinear, normalize(R), mipLevel).rgb;
    
    float NdotV = dot(normal.xyz, eyeVec);
    float2 BRDF = gBRDFLookUpTableBuffer.Sample(g_samLinearClamp, float2(max(NdotV, 0.f), metallicRoughnessAOSpecular.y)).rg;


    float3 specularIBL = prefilteredColor * (c_spec * BRDF.x + BRDF.y);
    Output.xyz += specularIBL;

    // gamma correction
    Output.xyz = pow(Output.xyz, 1.f / 2.0f);
    Output.w = albedo.w;
    
    return Output;
}
