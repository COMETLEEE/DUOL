#include "ConstantBuffer.hlsli"
#include "OIT_Header.hlsli"

Texture2D gDepthBuffer : register(t1); // ��ƼŬ�� �׸� �� �տ� ������Ʈ�� ������ �׸��� �ʱ� ���ؼ� �������۸� �����Ѵ�.
// ������ ���� ��, �������� ������Ʈ�� �׸� �������̴�.
Texture2D gPreDepthBuffer : register(t2); // ��ƼŬ�� �׸� �� �տ� ������Ʈ�� ������ �׸��� �ʱ� ���ؼ� �������۸� �����Ѵ�.
// OIT Layer�� �׸� �� ���� ���� ���̴�.

RWStructuredBuffer<PixelNode> gPixelLinkBuffer : register(u0); // ���� ����Ʈ�� ����� ������ �ϴ� ����. 1920 * 1080 * ������ �ȼ� �� �� ������� �Ҵ��صд�.
RWByteAddressBuffer gFirstOffsetBuffer : register(u1); // �ε��� ����. ���������� ��ϵ� �ȼ��� �ε��� ���� �����ϰ� �ִ�. ����Ʈ �������� ����� �ȼ��� �����Ѵ�.


struct VertexOut
{
    float4 PosH : SV_POSITION;
    float3 PosW : POSITION;
    float3 NormalW : NORMAL;
    float4 Color : COLOR;
    float2 Tex : TEXCOORD;
    float3 tangent : TANGENT;
};

struct OITPSOut
{
    float4 Color : SV_Target;
    float Depth : SV_Depth;
};
DeferredPSOut main(VertexOut pin) : SV_TARGET
{

    DeferredPSOut _psOut;
    _psOut.Position = float4(pin.PosW, 1.0f);
    _psOut.Depth = float4(pin.PosH.zzz, 1.0f);
    _psOut.Albedo = gColor;
#ifdef NOLIGHT
     _psOut.Normal = float4(0.0f,0.0f,0.0f, 1.0f);
#else
    _psOut.Normal = float4(pin.NormalW, 1.0f);
#endif
    
    _psOut.MetalicRoughnessAosPecular = float4(1.0f, 1.0f, 1.0f, 1.0f);
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


void OIT_BasicLight_PS(VertexOut pin) // �ȼ��� �����ϴ� pixel shader
{
    const float2 posTexCoord = pin.PosH.xy / gScreenXY;
    
    float4 depth = gDepthBuffer.Sample(samAnisotropic, posTexCoord);
    
    clip(gColor.a - 0.0001f);
    
    if ((depth.x >= 0.001f))
    {
        clip(depth.x - pin.PosH.z - EPSILON);
    }
    
    uint pixelCount = gPixelLinkBuffer.IncrementCounter(); // ī��Ʈ�� ����Ѵ�.
    
    uint2 vPos = (uint2) pin.PosH.xy;
    
    uint startOffsetAddress = 4 * (gScreenXY.x * vPos.y + vPos.x);
    uint oldStartOffset;
    
    // FirstOffsetBuffer�� ȭ�鿡 ���������� ��ϵ� �ȼ��� �ε��� ���� �����ϴ� �����̴�..
    gFirstOffsetBuffer.InterlockedExchange(
        startOffsetAddress, pixelCount, oldStartOffset);
    
    PixelNode node;
    node.Data.Color = PackColorFromFloat4(gColor);
    node.Data.Depth = pin.PosH.z;
    node.Data.BlendType = 0;
    node.Next = oldStartOffset;
    
    gPixelLinkBuffer[pixelCount] = node;
}