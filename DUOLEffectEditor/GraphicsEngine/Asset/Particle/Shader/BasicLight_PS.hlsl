#include "ConstantBuffer.hlsli"

Texture2D gDepthBuffer : register(t1); // ��ƼŬ�� �׸� �� �տ� ������Ʈ�� ������ �׸��� �ʱ� ���ؼ� �������۸� �����Ѵ�.
// ������ ���� ��, �������� ������Ʈ�� �׸� �������̴�.
Texture2D gPreDepthBuffer : register(t2); // ��ƼŬ�� �׸� �� �տ� ������Ʈ�� ������ �׸��� �ʱ� ���ؼ� �������۸� �����Ѵ�.
// OIT Layer�� �׸� �� ���� ���� ���̴�.

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
	//// ���� ������ ������ �� �̻� �������Ͱ� �ƴ� �� �����Ƿ� �ٽ� ����ȭ�Ѵ�.
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
	///// ť�� ����, ȯ�� ��. ���� todo ���߿� �ٽ� �غ���~
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

    clip(curDepth - preDepth - EPSILON); // ���� ���̾��� ������ ���ϱ� ����. ���� �ڿ� ���� ��쿡 �׸��� �տ� �ִٸ� �� �̻� �׸��� �ʾƵ� �ȴ�.

    OITPSOut output;

    output.Color = gColor;
    
    clip(output.Color.a - EPSILON);

    output.Depth = curDepth;

    return output;

}