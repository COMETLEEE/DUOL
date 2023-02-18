#include "ConstantBuffer.hlsli"
#include "OIT_Header.hlsli"

Texture2D gDepthBuffer : register(t1); // 파티클을 그릴 때 앞에 오브젝트가 있으면 그리지 않기 위해서 뎁스버퍼를 참조한다.
// 기존의 뎁스 값, 불투명한 오브젝트를 그린 뎁스값이다.
Texture2D gPreDepthBuffer : register(t2); // 파티클을 그릴 때 앞에 오브젝트가 있으면 그리지 않기 위해서 뎁스버퍼를 참조한다.
// OIT Layer를 그릴 때 생긴 뎁스 값이다.

RWStructuredBuffer<PixelNode> gPixelLinkBuffer : register(u0); // 정적 리스트의 저장소 역할을 하는 버퍼. 1920 * 1080 * 저장할 픽셀 수 의 사이즈로 할당해둔다.
RWByteAddressBuffer gFirstOffsetBuffer : register(u1); // 인덱스 버퍼. 마지막으로 기록된 픽셀의 인덱스 값을 저장하고 있다. 리스트 형식으로 저장된 픽셀을 추적한다.


struct VertexOut
{
    float4 PosH : SV_POSITION;
    float3 PosW : POSITION;
    float3 NormalW : NORMAL;
    float4 Color : COLOR;
    float2 Tex : TEXCOORD;
    float3 tangent : TANGENT;
};
struct PSOut
{
    float4 Depth : SV_Target0;
    float4 Normal : SV_Target1;
    float4 Position : SV_Target2;
    float4 Albedo : SV_Target3;
    float4 Diffuse : SV_Target4;
    float4 Specular : SV_Target5;
    float4 ObejctID : SV_Target6;
};
struct OITPSOut
{
    float4 Color : SV_Target;
    float Depth : SV_Depth;
};
PSOut main(VertexOut pin) : SV_TARGET
{
	//// 보간 때문에 법선이 더 이상 단위벡터가 아닐 수 있으므로 다시 정규화한다.
 //   pin.NormalW = normalize(pin.NormalW);
 //   float3 toEyeW = normalize(gEyePosW - pin.PosW);
 //   float4 litColor = pin.Color;
 //   float3 normalMapSample = float3(1, 1, 1);
 //   float3 NormalW = float3(1.0f, 1.0f, 1.0f);
 //   if (gUseTexure)
 //   {
 //       litColor = gDiffuseMap.Sample(samAnisotropic, pin.Tex);
 //   }
 //   if (gUseNormalMap)
 //   {
 //       normalMapSample = gNormalMap.Sample(samAnisotropic, pin.Tex).rgb;
 //       NormalW = NormalSampleToWorldSpace(normalMapSample, pin.NormalW, pin.tangent);
 //   }
 //   else
 //   {
 //       NormalW = pin.NormalW;
 //   }
	///// 큐브 맵핑, 환경 맵. 적용 todo 나중에 다시 해보자~
	//{
 //       float3 incident = -toEyeW;
 //       float3 reflectionVector = reflect(incident, pin.NormalW);
 //       float4 reflectionColor = gCubeMap.Sample(samAnisotropic, reflectionVector);
 //       litColor += reflectionColor;
 //   }

    PSOut _psOut;
    _psOut.Position = float4(pin.PosW, 1.0f);
    _psOut.Depth = float4(pin.PosH.zzz, 1.0f);
    _psOut.Albedo = gColor;
#ifdef NOLIGHT
     _psOut.Normal = float4(0.0f,0.0f,0.0f, 1.0f);
#else
    _psOut.Normal = float4(pin.NormalW, 1.0f);
#endif
    
    _psOut.Diffuse = float4(1.0f, 1.0f, 1.0f, 1.0f);
    _psOut.Specular = float4(1.0f, 1.0f, 1.0f, 1.0f);
    _psOut.ObejctID = ObjectID;

    return _psOut;
}

OITPSOut DrawDepthPeelingPS(VertexOut pin)
{
    const float2 posTexCoord = pin.PosH.xy / gScreenXY;

    float curDepth = pin.PosH.z;

    float4 depth = gDepthBuffer.Sample(samAnisotropic, posTexCoord);

    if ((depth.x >= EPSILON))
    {
        clip(depth.x - curDepth - EPSILON);
    }
    float preDepth = gPreDepthBuffer.Sample(samAnisotropic, posTexCoord).x;

    clip(curDepth - preDepth - EPSILON); // 이전 레이어의 뎁스와 비교하기 위함. 더욱 뒤에 있을 경우에 그리고 앞에 있다면 더 이상 그리지 않아도 된다.

    OITPSOut output;

    output.Color = gColor;
    
    clip(output.Color.a - EPSILON);

    output.Depth = curDepth;

    return output;

}


void OIT_BasicLight_PS(VertexOut pin) // 픽셀을 저장하는 pixel shader
{
    const float2 posTexCoord = pin.PosH.xy / gScreenXY;
    
    float4 depth = gDepthBuffer.Sample(samAnisotropic, posTexCoord);
    
    clip(gColor.a - 0.0001f);
    
    if ((depth.x >= 0.001f))
    {
        clip(depth.x - pin.PosH.z - EPSILON);
    }
    
    uint pixelCount = gPixelLinkBuffer.IncrementCounter(); // 카운트를 기록한다.
    
    uint2 vPos = (uint2) pin.PosH.xy;
    
    uint startOffsetAddress = 4 * (gScreenXY.x * vPos.y + vPos.x);
    uint oldStartOffset;
    
    // FirstOffsetBuffer는 화면에 마지막으로 기록된 픽셀의 인덱스 값을 저장하는 버퍼이다..
    gFirstOffsetBuffer.InterlockedExchange(
        startOffsetAddress, pixelCount, oldStartOffset);
    
    PixelNode node;
    node.Data.Color = PackColorFromFloat4(gColor);
    node.Data.Depth = pin.PosH.z;
    node.Data.BlendType = 0;
    node.Next = oldStartOffset;
    
    gPixelLinkBuffer[pixelCount] = node;
}