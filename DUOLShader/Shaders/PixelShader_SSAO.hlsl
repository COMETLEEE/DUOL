#include "ConstantBuffer.hlsli"

//--------------------------------------------------------------------------------------
// Globals
//--------------------------------------------------------------------------------------
#define SSAO_KERNEL_SIZE 64
#define radius 0.5f
#define bias 0.025f
// PerObject
cbuffer cbPerObject : register(b2)
{
    float3 SSAO_KERNEL[SSAO_KERNEL_SIZE];
};

//--------------------------------------------------------------------------------------
// Textures and Samplers
//--------------------------------------------------------------------------------------

Texture2D g_PosW : register(t0);
Texture2D g_Normal : register(t1);
Texture2D g_RotationSample : register(t2);

SamplerState g_samLinear : register(s0);
SamplerState g_samMirror : register(s1);

//--------------------------------------------------------------------------------------
// Input / Output structures
//--------------------------------------------------------------------------------------

struct PS_IN
{
    float4 PosH : SV_POSITION;
    float2 Texcoord0 : TEXCOORD0;
};

struct PS_OUT
{
    float SSAO;
};

//--------------------------------------------------------------------------------------
// Functions
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float PSMain(PS_IN Input) : SV_TARGET
{
    float2 noiseScale = float2(g_ScreenXY / 4.f);
    float4 posW = float4(g_PosW.Sample(g_samLinear, Input.Texcoord0).xyz, 1.f);
    float4 posV = mul(posW, g_Camera.g_ViewMatrix);

    // 텍스쳐의 각 픽셀이 노이즈맵에 반복되서 적용될수 있도록...
    float3 noise = g_RotationSample.Sample(g_samMirror, Input.Texcoord0  * noiseScale).xyz;

    // 노이즈맵의 회전벡터를 탄젠트값으로 사용합니다.
    // 그람슈미트를 사용한 tangent 구하기
    float3 normal = normalize(g_Normal.Sample(g_samLinear, Input.Texcoord0).xyz);
    // 카메라에는 아마 스케일이 없을것이다.. 없을것이다....... 역전치행렬은 아니지만 그냥 곱한다. 어짜피 로테이션만인데?
    normal = mul(normal, (float3x3)g_Camera.g_ViewMatrix);
    //뷰스페이스의 노말을 사용합니다.

    //여기서 의문. tanget를 구할떄 noise의 값은 탄젠트 스페이스 내부의 값이고 노말은 뷰스페이스인데..?
    float3 tangent = normalize(noise - normal * dot(noise, normal.xyz));
    float3 bitangent = normalize(cross(normal, tangent));
    float3x3 tbn = float3x3(tangent.xyz, bitangent.xyz, normal.xyz);

    float occlusion = 0.f;

    for (int i = 0; i < SSAO_KERNEL_SIZE; ++i)
    {
        // get sample position
        //반구의 샘플커널들을 가져와서 tbn에 곱해주어 계산한다.
        float3 sample = mul(SSAO_KERNEL[i], tbn); // From tangent to view-space
        float3 movedPos = posV.xyz + sample * radius;

        // 화면공간상에 투영한다
        float4 offset = float4(movedPos, 1.f);
        offset = mul(offset, g_Camera.g_ProjectionMatix);

        //동차좌표게 나누기
        offset.xyz /= offset.w;

        // 클립스페이스로 이동
        offset.xy = offset.xy * 0.5f + 0.5;
        offset.y = 1.f - offset.y;

        float4 sampledPosW = g_PosW.Sample(g_samLinear, offset.xy).xyzw;
        // 렌더되지 않은곳의 데이터는 스킵한다.
        if (sampledPosW.w <= 0.f)
        {
            continue;
        }
        float sampledDepth = mul(float4(sampledPosW.xyz, 1.f), g_Camera.g_ViewMatrix).z;
        // depth의 차이가 radius보다 작을수록 occlusion(차폐도는 높다고 계산한다.)
        // 또한, 차이가 멀수록 차폐도의 영향을 주지않는다.
        float rangeCheck = smoothstep(0.0f, 1.0f, radius / abs(posV.z - sampledDepth));

        // 텍스쳐의 월드포지션의 뎁스가 현재의 sample depth보다 가까울수록 차폐의 가능성이 높다.
        // DirectX에서는 View-Space에서 더 작을수록 가깝다.
        // 자기 근처의 slope같은 거에 영향을 받지 않기위해 bias를 추가한다.
        occlusion += (sampledDepth + bias <= movedPos.z  ? 1.0f : 0.0f) * rangeCheck;
    }

    occlusion = 1.0 - (occlusion / SSAO_KERNEL_SIZE);

    return occlusion;
}