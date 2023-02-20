Texture2D g_HDRTexture;

#define PI 3.141592f

SamplerState g_samLinear : register(s0);

//--------------------------------------------------------------------------------------
// Globals
//--------------------------------------------------------------------------------------
//// PerObject

cbuffer cbPerObject : register(b1)
{
    int face;
}
//--------------------------------------------------------------------------------------
// Input / Output structures
//--------------------------------------------------------------------------------------
float3 uvToXYZ(int face, float2 uv)
{
    if (face == 0)
        return float3(+1.f, +uv.y, -uv.x);
    else if (face == 1)
        return float3(-1.f, +uv.y, +uv.x);
    else if (face == 2)
        return float3(+uv.x, +1.f, -uv.y);
    else if (face == 3)
        return float3(+uv.x, -1.f, +uv.y);
    else if (face == 4)
        return float3(+uv.x, +uv.y, +1.f);
    else
        return float3(-uv.x, +uv.y, -1.f);
}

//HDR TO LDR
float3 Uncharted2Kernel(float3 x)
{
    float A = 0.15;
    float B = 0.50;
    float C = 0.10;
    float D = 0.20;
    float E = 0.02;
    float F = 0.30;
    return ((x * (A * x + C * B) + D * E) / (x * (A * x + B) + D * F)) - E / F;
}

float4 Uncharted2(float3 texColor)
{
    float g_fHardExposure = 1.5f;

    float W = 11.2;
    texColor *= g_fHardExposure; // Hardcoded Exposure Adjustment

    float ExposureBias = 2.0f;
    float3 curr = Uncharted2Kernel(ExposureBias * texColor);

    float3 whiteScale = 1.0f / Uncharted2Kernel(W);
    float3 color = curr * whiteScale;

    float3 retColor = pow(color, 1 / 2.2);
    return float4(retColor, 1);
}

float2 dirToUV(float3 dir)
{
    return float2(
		atan2(dir.x, dir.z) * 0.5f / PI + 0.5f,
		-asin(dir.y) / PI + 0.5f);
}

struct VS_INPUT
{
    float3 Position : POSITION;
    float2 Texcoord0 : TEXCOORD0;
};

struct VS_OUTPUT
{
    float4 PosH : SV_POSITION;
    float2 Texcoord0 : TEXCOORD0;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VSMain(VS_INPUT Input)
{
    VS_OUTPUT outp;
    
    outp.PosH = float4(Input.Position, 1.f);
    outp.Texcoord0 = Input.Texcoord0;
    
    return outp;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
struct VertexOut
{
    float4 PosH : SV_POSITION;
    float2 Texcoord0 : TEXCOORD0;
};

float4 PSMain(VertexOut pin) : SV_Target
{
    // x, y 다른 기준으로 -1 ~ 1 사이로 돌립니다.
    float2 uv = (pin.Texcoord0 - float2(0.5f, 0.5f)) * 2.f;
    uv *= float2(1.f, -1.f);
    
    float3 scan = uvToXYZ(face, uv);
    float3 direction = normalize(scan);
    float2 src = dirToUV(direction);

    float4 color = g_HDRTexture.Sample(g_samLinear, src);
    float4 LDRColor = Uncharted2(color.xyz);

    return color;
}
