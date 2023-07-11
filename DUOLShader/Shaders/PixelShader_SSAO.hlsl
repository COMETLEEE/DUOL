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

    // �ؽ����� �� �ȼ��� ������ʿ� �ݺ��Ǽ� ����ɼ� �ֵ���...
    float3 noise = g_RotationSample.Sample(g_samMirror, Input.Texcoord0  * noiseScale).xyz;

    // ��������� ȸ�����͸� ź��Ʈ������ ����մϴ�.
    // �׶�����Ʈ�� ����� tangent ���ϱ�
    float3 normal = normalize(g_Normal.Sample(g_samLinear, Input.Texcoord0).xyz);
    // ī�޶󿡴� �Ƹ� �������� �������̴�.. �������̴�....... ����ġ����� �ƴ����� �׳� ���Ѵ�. ��¥�� �����̼Ǹ��ε�?
    normal = mul(normal, (float3x3)g_Camera.g_ViewMatrix);
    //�佺���̽��� �븻�� ����մϴ�.

    //���⼭ �ǹ�. tanget�� ���ҋ� noise�� ���� ź��Ʈ �����̽� ������ ���̰� �븻�� �佺���̽��ε�..?
    float3 tangent = normalize(noise - normal * dot(noise, normal.xyz));
    float3 bitangent = normalize(cross(normal, tangent));
    float3x3 tbn = float3x3(tangent.xyz, bitangent.xyz, normal.xyz);

    float occlusion = 0.f;

    for (int i = 0; i < SSAO_KERNEL_SIZE; ++i)
    {
        // get sample position
        //�ݱ��� ����Ŀ�ε��� �����ͼ� tbn�� �����־� ����Ѵ�.
        float3 sample = mul(SSAO_KERNEL[i], tbn); // From tangent to view-space
        float3 movedPos = posV.xyz + sample * radius;

        // ȭ������� �����Ѵ�
        float4 offset = float4(movedPos, 1.f);
        offset = mul(offset, g_Camera.g_ProjectionMatix);

        //������ǥ�� ������
        offset.xyz /= offset.w;

        // Ŭ�������̽��� �̵�
        offset.xy = offset.xy * 0.5f + 0.5;
        offset.y = 1.f - offset.y;

        float4 sampledPosW = g_PosW.Sample(g_samLinear, offset.xy).xyzw;
        // �������� �������� �����ʹ� ��ŵ�Ѵ�.
        if (sampledPosW.w <= 0.f)
        {
            continue;
        }
        float sampledDepth = mul(float4(sampledPosW.xyz, 1.f), g_Camera.g_ViewMatrix).z;
        // depth�� ���̰� radius���� �������� occlusion(���󵵴� ���ٰ� ����Ѵ�.)
        // ����, ���̰� �ּ��� ������ ������ �����ʴ´�.
        float rangeCheck = smoothstep(0.0f, 1.0f, radius / abs(posV.z - sampledDepth));

        // �ؽ����� ������������ ������ ������ sample depth���� �������� ������ ���ɼ��� ����.
        // DirectX������ View-Space���� �� �������� ������.
        // �ڱ� ��ó�� slope���� �ſ� ������ ���� �ʱ����� bias�� �߰��Ѵ�.
        occlusion += (sampledDepth + bias <= movedPos.z  ? 1.0f : 0.0f) * rangeCheck;
    }

    occlusion = 1.0 - (occlusion / SSAO_KERNEL_SIZE);

    return occlusion;
}