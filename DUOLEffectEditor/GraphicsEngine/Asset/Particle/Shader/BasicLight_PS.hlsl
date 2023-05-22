#include "ConstantBuffer.hlsli"
#include "OIT_Header.hlsli"

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
struct OITOut
{
    float4 OverColor : SV_Target0;
    float4 OverInfo : SV_Target1;
    float4 AdditiveColor : SV_Target2;
    float4 AdditiveInfo : SV_Target3;
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
    
    _psOut.MetalicRoughnessAoSpecular = gMetalicRoughnessAoSpecular;
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


OITOut OIT_BasicLight_PS(VertexOut pin) // �ȼ��� �����ϴ� pixel shader
{
    OITOut oitOut;
    
    oitOut.OverColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
    oitOut.OverInfo = float4(0.0f, 0.0f, 0.0f, 0.0f);
    oitOut.AdditiveColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
    oitOut.AdditiveInfo = float4(0.0f, 0.0f, 0.0f, 0.0f);
    
    const float2 posTexCoord = pin.PosH.xy / gScreenXY;
    
    float4 depth = gDepthBuffer.Sample(samAnisotropic, posTexCoord);
    
    clip(gColor.a - 0.0001f);
    
    if ((depth.x >= 0.001f))
    {
        clip(depth.x - pin.PosH.z - EPSILON);
    }
    
    uint2 vPos = (uint2) pin.PosH.xy;
    
    float currentDepth = pin.PosH.z * pin.PosH.w;
    
    uint offset = 4 * (gScreenXY.x * vPos.y + vPos.x);
    
        // near : 1, far : 300  500/300 // 0.1~500 �� ���� Z ���̴�.
    float z = (499.9f / 299.0f) * (currentDepth - 1) + 0.1f;
    float w = gColor.w * max(0.01f, min(3000.0f, 0.03f / (0.00001f + pow(abs(z) / 200.0f, 4.0f))));
        
    oitOut.OverColor = float4((gColor.xyz) * gColor.a, gColor.a) * w;
    oitOut.OverInfo.x = currentDepth;
    oitOut.OverInfo.y += 1.0f;
    oitOut.OverInfo.z = 1 - gColor.a;
    
    return oitOut;

}