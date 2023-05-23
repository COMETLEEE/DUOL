#include "ConstantBuffer.hlsli"
#include "ParticleHelper.hlsli"

Texture2DArray gTexArray : register(t0);

//Texture2D gDepthBuffer : register(t1); // ��ƼŬ�� �׸� �� �տ� ������Ʈ�� ������ �׸��� �ʱ� ���ؼ� �������۸� �����Ѵ�.
// ������ ���� ��, �������� ������Ʈ�� �׸� �������̴�.
//Texture2D gPreDepthBuffer : register(t2); // ��ƼŬ�� �׸� �� �տ� ������Ʈ�� ������ �׸��� �ʱ� ���ؼ� �������۸� �����Ѵ�.
// OIT Layer�� �׸� �� ���� ���� ���̴�.

SamplerState samAnisotropic : register(s0);

struct OITOut
{
    float4 OverColor : SV_Target0;
    float4 OverInfo : SV_Target1;
    float4 AdditiveColor : SV_Target2;
    float4 AdditiveInfo : SV_Target3;
};

struct GeoOut
{
    float4 PosH : SV_Position;
    float4 Color : COLOR;
    float2 Tex : TEXCOORD1;
};

[earlydepthstencil]
OITOut OIT_Particle_PS(GeoOut pin) // �ȼ��� �����ϴ� pixel shader
{
    OITOut oitOut;
    
    oitOut.OverColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
    oitOut.OverInfo = float4(0.0f, 0.0f, 0.0f, 1.0f);
    oitOut.AdditiveColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
    oitOut.AdditiveInfo = float4(0.0f, 0.0f, 0.0f, 1.0f);

    float4 texColor = gTexArray.Sample(samAnisotropic, float3(pin.Tex, 0)) * pin.Color;

    if (texColor.a <= 0.000001f)
        clip(-1.0f);

    const float2 posTexCoord = pin.PosH.xy / g_ScreenXY;

    float currentDepth = pin.PosH.z * pin.PosH.w;
    
    if (gParticleRenderer.gBlendType == 0) // Over
    {
        float z = currentDepth;
        float w = texColor.w * max(0.01f, min(3000.0f, 0.03f / (0.00001f + pow(abs(z) / 200.0f, 4.0f))));
        
        oitOut.OverColor = float4((texColor.xyz) * texColor.a, texColor.a) * w;
        oitOut.OverInfo.x = currentDepth;
        oitOut.OverInfo.y = 1.0f;
        oitOut.OverInfo.z = texColor.a * w;
        oitOut.OverInfo.a = 1 - texColor.a;
    }
    else // Additive
    {
        oitOut.AdditiveColor = float4((texColor.xyz) * texColor.a, texColor.a);
        oitOut.AdditiveInfo.x = currentDepth;
        oitOut.AdditiveInfo.y = 1.0f;
        oitOut.AdditiveInfo.z = texColor.a;
        oitOut.AdditiveInfo.a = 1 - texColor.a;
    }
    
    return oitOut;
}