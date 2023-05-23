#include "ConstantBuffer.hlsli"

Texture2D ObjectIDTexture;
Texture2D NormalTexture;
Texture2D SceneViewTexture;

cbuffer cbPerObject : register(b2)
{
    float2 g_ObjectID;
}

struct VertexOut
{
    float4 PosH : SV_POSITION;
    float2 UV : TEXCOORD0;
};

SamplerState g_samLinear : register(s0);

static const float offX = (1.0f / float(g_ScreenXY.x));
static const float offY = (1.0f / float(g_ScreenXY.y));

float4 PSMain(VertexOut pin) : SV_Target
{
    // �ش� �ȼ��� ObjectID
    float4 currentObjectID = ObjectIDTexture.Sample(g_samLinear, pin.UV);

    float4 currentObjectID00 = ObjectIDTexture.Sample(g_samLinear, pin.UV + float2(-offX, -offY));
    float4 currentObjectID01 = ObjectIDTexture.Sample(g_samLinear, pin.UV + float2(0.f, -offY));
    float4 currentObjectID02 = ObjectIDTexture.Sample(g_samLinear, pin.UV + float2(offX, -offY));
    float4 currentObjectID10 = ObjectIDTexture.Sample(g_samLinear, pin.UV + float2(-offX, 0.f));
    float4 currentObjectID12 = ObjectIDTexture.Sample(g_samLinear, pin.UV + float2(offX, 0.f));
    float4 currentObjectID20 = ObjectIDTexture.Sample(g_samLinear, pin.UV + float2(-offX, offY));
    float4 currentObjectID21 = ObjectIDTexture.Sample(g_samLinear, pin.UV + float2(0.f, offY));
    float4 currentObjectID22 = ObjectIDTexture.Sample(g_samLinear, pin.UV + float2(offX, offY));

    // ���õ� ���� ������Ʈ�� ���̵� �ȼ��Դϴ�.
    if ((currentObjectID.x == g_ObjectID.x) && (currentObjectID.y == g_ObjectID.y))
    {
        // �ֺ��� �ϳ��� �ٸ� ������Ʈ ���̵� �ȼ��� �ִٸ� �ش� ������Ʈ�� �ܰ����Դϴ�.
        if (((currentObjectID00.x != g_ObjectID.x) || (currentObjectID00.y != g_ObjectID.y))
		|| ((currentObjectID01.x != g_ObjectID.x) || (currentObjectID01.y != g_ObjectID.y))
		|| ((currentObjectID02.x != g_ObjectID.x) || (currentObjectID02.y != g_ObjectID.y))
		|| ((currentObjectID10.x != g_ObjectID.x) || (currentObjectID10.y != g_ObjectID.y))
		|| ((currentObjectID12.x != g_ObjectID.x) || (currentObjectID12.y != g_ObjectID.y))
		|| ((currentObjectID20.x != g_ObjectID.x) || (currentObjectID20.y != g_ObjectID.y))
		|| ((currentObjectID21.x != g_ObjectID.x) || (currentObjectID21.y != g_ObjectID.y))
		|| ((currentObjectID22.x != g_ObjectID.x) || (currentObjectID22.y != g_ObjectID.y)))
            return float4(1.f, 0.5f, 0.f, 1.f);
        else
            return float4(0.f, 0.f, 0.f, 0.f);
    }

    return float4(0.f, 0.f, 0.f, 0.f);
}