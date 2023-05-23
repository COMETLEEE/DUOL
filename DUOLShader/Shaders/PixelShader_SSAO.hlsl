#include "ConstantBuffer.hlsli"

Texture2D ObjectIDTexture;
Texture2D NormalTexture;
Texture2D PositionTexture;
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

#pragma region RIM_LIGHTING
float4 PSMain(VertexOut pin) : SV_Target
{
    float4 currentObjectID = ObjectIDTexture.Sample(g_samLinear, pin.UV);

    float4 normal = NormalTexture.Sample(g_samLinear, pin.UV);

    float4 worldPos = PositionTexture.Sample(g_samLinear, pin.UV);

    float4 sceneColor = SceneViewTexture.Sample(g_samLinear, pin.UV);

    if ((currentObjectID.x != g_ObjectID.x) || (currentObjectID.y != g_ObjectID.y))
        return float4(0.f, 0.f, 0.f, 0.f);

    float rim = 0.f;

    rim = 1.f - saturate(dot(normal.xyz, normalize(g_Camera.g_CameraPosition - worldPos.xyz)));
    
    rim = pow(rim, 5.f);

    return sceneColor + float4(rim * 0.8f, rim * 0.2f, rim * 0.1f, 1.f);
}
#pragma endregion

#pragma region EDGE_DETECTING
//// ObjectID를 비교하여 Outline이 그려진 RenderTarget을 추가합니다 ..?
//float4 PSMain(VertexOut pin) : SV_Target
//{
//    float4 currentObjectID = ObjectIDTexture.Sample(g_samLinear, pin.UV);

//    float4 normal = NormalTexture.Sample(g_samLinear, pin.UV);

//    float4 worldPos = PositionTexture.Sample(g_samLinear, pin.UV);

//    float4 sceneColor = SceneViewTexture.Sample(g_samLinear, pin.UV);

//    if ((currentObjectID.x != g_ObjectID.x) || (currentObjectID.y != g_ObjectID.y))
//        return float4(0.f, 0.f, 0.f, 0.f);

//    float rim = 0.f;

//    rim = 1.f - saturate(dot(normal.xyz, normalize(g_Camera.g_CameraPosition - worldPos.xyz)));
    
//    rim = pow(rim, 5.f);

//    return sceneColor + float4(rim * 0.8f, rim * 0.2f, rim * 0.1f, 1.f);
//}
#pragma endregion