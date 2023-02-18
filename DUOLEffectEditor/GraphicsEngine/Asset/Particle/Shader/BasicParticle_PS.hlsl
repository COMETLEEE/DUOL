#include "ConstantBuffer.hlsli"
#include "OIT_Header.hlsli"

Texture2DArray gTexArray : register(t0);

Texture2D gDepthBuffer : register(t1); // ��ƼŬ�� �׸� �� �տ� ������Ʈ�� ������ �׸��� �ʱ� ���ؼ� �������۸� �����Ѵ�.
// ������ ���� ��, �������� ������Ʈ�� �׸� �������̴�.
Texture2D gPreDepthBuffer : register(t2); // ��ƼŬ�� �׸� �� �տ� ������Ʈ�� ������ �׸��� �ʱ� ���ؼ� �������۸� �����Ѵ�.
// OIT Layer�� �׸� �� ���� ���� ���̴�.

RWStructuredBuffer<PixelNode> gPixelLinkBuffer : register(u0); // ���� ����Ʈ�� ����� ������ �ϴ� ����. 1920 * 1080 * ������ �ȼ� �� �� ������� �Ҵ��صд�.
RWByteAddressBuffer gFirstOffsetBuffer : register(u1); // �ε��� ����. ���������� ��ϵ� �ȼ��� �ε��� ���� �����ϰ� �ִ�. ����Ʈ �������� ����� �ȼ��� �����Ѵ�.

struct PSOut
{
    float4 Color : SV_Target;
    float Depth : SV_Depth;
};
struct GeoOut
{
    float4 PosH : SV_Position;
    float4 Color : COLOR;
    float2 Tex : TEXCOORD1;
};
//#define Draw_Depth 1

float4 DrawPS(GeoOut pin) : SV_Target
{
    const float2 posTexCoord = pin.PosH.xy / gScreenXY;

    float4 depth = gDepthBuffer.Sample(samAnisotropic, posTexCoord);

    float4 Color = gTexArray.Sample(samAnisotropic, float3(pin.Tex, 0)) * pin.Color;
    
    if ((depth.x >= 0.001f))
    {
        clip(depth.x - pin.PosH.z - EPSILON);
    }
    
    clip(Color.a - EPSILON);
    
#ifdef Draw_Object_ID
    return float4(gCommonInfo.gObjectID, 0, 0, 1.0f);
#elif Draw_Depth
    return float4(pin.PosH.z, pin.PosH.z, pin.PosH.z, pin.PosH.z);
#else
    return Color;
#endif
}
float4 DrawOutLine(GeoOut pin) : SV_Target
{
    return float4(1.0f, 1.0f, 0.0f, 1.0f);
}

PSOut DrawDepthPeelingPS(GeoOut pin) // 1�� ������ �����ߴ� �����ʸ� ����� OIT ����ȭ�� ���� ������..!
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

    PSOut output;

    output.Color = gTexArray.Sample(samAnisotropic, float3(pin.Tex, 0)) * pin.Color;

    clip(output.Color.a - EPSILON);

    output.Depth = curDepth;

    return output;

}

void OIT_Particle_PS(GeoOut pin) // �ȼ��� �����ϴ� pixel shader
{
    float4 texColor = gTexArray.Sample(samAnisotropic, float3(pin.Tex, 0)) * pin.Color;
    
    const float2 posTexCoord = pin.PosH.xy / gScreenXY;

    float4 depth = gDepthBuffer.Sample(samAnisotropic, posTexCoord);
    
    clip(texColor.a - 0.0001f);

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
    
    float strength = length(texColor);
    float4 color = texColor;
    
    PixelNode node;
    node.Data.Color = PackColorFromFloat4(color);
    node.Data.Depth = pin.PosH.z;
    node.Data.BlendType = gParticleRenderer.gBlendType;
    node.Next = oldStartOffset;
    
    gPixelLinkBuffer[pixelCount] = node;
}