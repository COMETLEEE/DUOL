TextureCube g_CubeMap;

#define PI 3.141592
#define TwoPI 2 * PI
#define Epsilon 0.00001

SamplerState g_samLinear : register(s1);


cbuffer cbPerObject : register(b1)
{
    int face;
    float3 pad;
}

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

struct VS_OUT
{
    float4 pos : SV_POSITION;
    float2 uv : UV;
};


float4 PSMain(VS_OUT pin)
    : SV_Target
{
    // x, y 다른 기준으로 -1 ~ 1 사이로 돌립니다.
    float2 uv = (pin.uv - float2(0.5f, 0.5f)) * 2.f;
    uv *= float2(1.f, -1.f);

    float3 scan = uvToXYZ(face, uv);
    float3 normal = normalize(scan);

    float3 irradiance = float3(0.f, 0.f, 0.f);

    // Branchless select non-degenerate T.
    float3 right = cross(normal, float3(0.0, 1.0, 0.0));
    right = lerp(cross(normal, float3(1.0, 0.0, 0.0)), right, step(Epsilon, dot(right, right)));
    right = normalize(right);
    float3 up = cross(normal, right);

    float sampleDelta = 0.025;
    float nrSamples = 0.0;

    for (float phi = 0.0; phi < 2.0 * PI; phi += sampleDelta)
    {
        for (float theta = 0.0; theta < 0.5 * PI; theta += sampleDelta)
        {
            // spherical to cartesian (in tangent space)
            // 구면좌표계에서.. 어
            float3 tangentSample = float3(sin(theta) * sin(phi), sin(theta) * cos(phi), cos(theta));
            // tangent space to world
            float3 sampleVec = tangentSample.x * right + tangentSample.y * up + tangentSample.z * normal;

            irradiance += g_CubeMap.Sample(g_samLinear, sampleVec).rgb * cos(theta);
            nrSamples++;
        }
    }

    irradiance = PI * irradiance * (1.0 / float(nrSamples));

    return float4(irradiance, 1.f);
}
