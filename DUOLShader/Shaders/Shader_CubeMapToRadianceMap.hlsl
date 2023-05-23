TextureCube g_CubeMap;

#define PI 3.141592
#define TwoPI 2 * PI
#define Epsilon 0.00001

SamplerState g_samLinear : register(s0);

//--------------------------------------------------------------------------------------
// Globals
//--------------------------------------------------------------------------------------
//// PerObject

cbuffer cbPerObject : register(b2)
{
    int face;
    float roughness;
}
//--------------------------------------------------------------------------------------
// Input / Output structures
//--------------------------------------------------------------------------------------
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

float3 uvToXYZ(int face, float2 uv)
{
    if (face == 0)
        return float3(+1.f, uv.y, -uv.x);
    else if (face == 1)
        return float3(-1.f, uv.y, +uv.x);
    else if (face == 2)
        return float3(+uv.x, +1.f, -uv.y);
    else if (face == 3)
        return float3(+uv.x, -1.f, +uv.y);
    else if (face == 4)
        return float3(+uv.x, uv.y, +1.f);
    else
        return float3(-uv.x, uv.y, -1.f);
}

float RadicalInverse_VdC(uint bits)
{
    bits = (bits << 16u) | (bits >> 16u);
    bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
    bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
    bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
    bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
    return float(bits) * 2.3283064365386963e-10; // / 0x100000000
}
// ----------------------------------------------------------------------------
float2 Hammersley(uint i, uint N)
{
    return float2(float(i) / float(N), RadicalInverse_VdC(i));
}

float3 ImportanceSampleGGX(float2 Xi, float3 N, float roughness)
{
    float a = roughness * roughness;

    float phi = 2.0 * PI * Xi.x;
    float cosTheta = sqrt((1.0 - Xi.y) / (1.0 + (a * a - 1.0) * Xi.y));
    float sinTheta = sqrt(1.0 - cosTheta * cosTheta);

    // from spherical coordinates to cartesian coordinates
    float3 H;
    H.x = cos(phi) * sinTheta;
    H.y = sin(phi) * sinTheta;
    H.z = cosTheta;

    // from tangent-space vector to world-space sample vector
    float3 up = abs(N.z) < 0.999 ? float3(0.0, 0.0, 1.0) : float3(1.0, 0.0, 0.0);
    float3 tangent = normalize(cross(up, N));
    float3 bitangent = cross(N, tangent);

    float3 sampleVec = tangent * H.x + bitangent * H.y + N * H.z;
    return normalize(sampleVec);
}

float NormalDistributionGGXTR(float3 normalVec, float3 halfwayVec, float Roughness2)
{
    float a2 = Roughness2 * Roughness2;                  // a2 = a^2
    float NdotH = max(dot(normalVec, halfwayVec), 0.0f); // NdotH = normalVec.halfwayVec
    float NdotH2 = NdotH * NdotH;                        // NdotH2 = NdotH^2
    float denom = (PI * pow(NdotH2 * (a2 - 1.0f) + 1.0f, 2.0f));

    if (denom < Epsilon)
        return 1.0f;

    return a2 / denom;
}

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

float3 Uncharted2(float3 texColor)
{
    float g_fHardExposure = 1.5f;

    float W = 11.2;
    texColor *= g_fHardExposure; // Hardcoded Exposure Adjustment

    float ExposureBias = 2.0f;
    float3 curr = Uncharted2Kernel(ExposureBias * texColor);

    float3 whiteScale = 1.0f / Uncharted2Kernel(W);
    float3 color = curr * whiteScale;

    return color;
}

float4 PSMain(VertexOut pin)
    : SV_Target
{
    // x, y 다른 기준으로 -1 ~ 1 사이로 돌립니다.
    float2 uv = float2(pin.Texcoord0.x, 1.f - pin.Texcoord0.y);
    uv = uv * 2.f - float2(1.f, 1.f);

    // 큐브맵의서의 좌표로.
    float3 scan = uvToXYZ(face, uv);
    float3 N = normalize(scan);
    float3 R = N;
    float3 V = R;

    float3 radiance = float3(0.f, 0.f, 0.f);
    const uint SAMPLE_COUNT = 1024u;
    float totalWeight = 0.0;

    float roughness2 = pow(roughness, 2);

    for (uint idx = 0u; idx < SAMPLE_COUNT; ++idx)
    {
        float2 Xi = Hammersley(idx, SAMPLE_COUNT);
        float3 H = ImportanceSampleGGX(Xi, N, roughness2);
        float3 L = normalize(2.0 * dot(V, H) * H - V);

        float NdotL = max(dot(N, L), 0.0);

        if (NdotL > 0.0)
        {
            radiance += Uncharted2(g_CubeMap.Sample(g_samLinear, L).rgb).xyz * NdotL;
            totalWeight += NdotL;

            // float D = NormalDistributionGGXTR(N, H, roughness2);
            // float NdotH = max(dot(N, H), 0.0f);
            // float HdotV = max(dot(H, N), 0.0f);
            // float pdf = (D * NdotH) / (4.0f * HdotV);

            // float resolution = 1024.0f;
            // float saTexel = 4.0f * PI / (6.0f * resolution * resolution);
            // float saSample = 1.0f / max((float(SAMPLE_COUNT) * pdf), Epsilon);

            // float fMipBias = 1.0f;
            // float fMipLevel = (roughness == 0.0) ? 0.0 : max(0.5 * log2(saSample / saTexel) + fMipBias, 0.0f);

            // radiance += g_CubeMap.SampleLevel(g_samLinear, L, fMipLevel).rgb * NdotL;
            // totalWeight += NdotL;

        }
    }

    radiance /= totalWeight;

    return float4(radiance, 1.f);
}
