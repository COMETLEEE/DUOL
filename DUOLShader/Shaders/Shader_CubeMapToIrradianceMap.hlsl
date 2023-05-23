TextureCube g_CubeMap;

#define PI 3.141592
#define TwoPI 2 * PI
#define INV_PI 1.f / PI
#define Epsilon 0.00001

SamplerState g_samLinear : register(s0);
SamplerState g_samTriPoint : register(s1);

//--------------------------------------------------------------------------------------
// Globals
//--------------------------------------------------------------------------------------
//// PerObject

cbuffer cbPerObject : register(b2)
{
    int face;
}
//--------------------------------------------------------------------------------------
// Input / Output structures
//--------------------------------------------------------------------------------------
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

//
// Attributed to:
// http://holger.dammertz.org/stuff/notes_HammersleyOnHemisphere.html
// Holger Dammertz.
//

float RadicalInverse_VdC(uint bits)
{
    bits = (bits << 16u) | (bits >> 16u);
    bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
    bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
    bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
    bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
    return float(bits) * 2.3283064365386963e-10; // / 0x100000000
}

float2 Hammersley(uint i, uint N)
{
    return float2(float(i) / float(N), RadicalInverse_VdC(i));
}

float3x3 QuaternionToMatrix(float4 quat)
{
    float3 cross = quat.yzx * quat.zxy;
    float3 square = quat.xyz * quat.xyz;
    float3 wimag = quat.w * quat.xyz;

    square = square.xyz + square.yzx;

    float3 diag = 0.5 - square;
    float3 a = (cross + wimag);
    float3 b = (cross - wimag);

    return float3x3(
        2.0 * float3(diag.x, b.z, a.y),
        2.0 * float3(a.z, diag.y, b.x),
        2.0 * float3(b.y, a.x, diag.z));
}

float3 rescaleHDR(float3 hdrPixel, float3 MaxRgb)
{
    if (hdrPixel.x < 0)
        hdrPixel.x = 0;
    if (hdrPixel.y < 0)
        hdrPixel.y = 0;
    if (hdrPixel.z < 0)
        hdrPixel.z = 0;

    float intensity = float(dot(hdrPixel, float3(0.299f, 0.587f, 0.114f)));

    float3 IblMaxValue = MaxRgb;
    // 
    // Saturation, Hue
    float3 IBLCorrection = float3(0.f, 1.0f, 0.f);
    float EnviromentScale = 2.0f;

    if (intensity > 1)
    {
        hdrPixel = hdrPixel - IBLCorrection.x * (hdrPixel - IblMaxValue.rgb) * hdrPixel * (hdrPixel - (IblMaxValue.rgb * 0.5));
    }
    else
    {
        hdrPixel = hdrPixel - IBLCorrection.x * (hdrPixel - IblMaxValue.rgb) * hdrPixel * (hdrPixel - 0.5);
    }

    // Saturation adjustment
    hdrPixel = lerp(intensity.xxx, hdrPixel, IBLCorrection.y);

    // Hue adjustment
    const float3 root = float3(0.57735, 0.57735, 0.57735);
    float half_angle = 0.5 * radians(IBLCorrection.z); // Hue is radians of 0 tp 360 degree
    float4 rot_quat = float4((root * sin(half_angle)), cos(half_angle));
    float3x3 rot_Matrix = QuaternionToMatrix(rot_quat);
    hdrPixel = mul(rot_Matrix, hdrPixel);
    hdrPixel = hdrPixel * EnviromentScale;

    return hdrPixel;
}

float3 GetMaxColorValue()
{
    float cubeWidth = 0;
    float cubeHeight = 0;
    float level = 0;

    float3 maxRgb = float3(0.f, 0.f, 0.f);
    
    g_CubeMap.GetDimensions(0, cubeWidth, cubeHeight, level);

    for (int arrayIdx = 0; arrayIdx < 6; arrayIdx++)
    {
        float3 uv;
        
        for (int pixelx = 0; pixelx < cubeWidth; pixelx++)
        {
            for (int pixely = 0; pixely < cubeHeight; pixely++)
            {
                if (arrayIdx == 0)
                    uv = float3(+1.f, +pixely / cubeHeight, -pixelx / cubeWidth);
                else if (arrayIdx == 1)
                    uv = float3(1.f, +pixely / cubeHeight, +pixelx / cubeWidth);
                else if (arrayIdx == 2)
                    uv = float3(+pixelx / cubeWidth, +1.f, -pixely / cubeHeight);
                else if (arrayIdx == 3)
                    uv = float3(+pixelx / cubeWidth, -1.f, +pixely / cubeHeight);
                else if (arrayIdx == 4)
                    uv = float3(+pixelx / cubeWidth, pixely / cubeHeight, +1.f);
                else
                    uv = float3(-pixelx / cubeWidth, pixely / cubeHeight, -1.f);
        

                float3 color = g_CubeMap.Sample(g_samLinear, uv);
                
                maxRgb = max(maxRgb, color);
            }
        }
    }
    
    return maxRgb;
}


float4 sumDiffuse(float3 diffuseSample, float NoV, float4 result)
{
    result.xyz += diffuseSample;
    result.w++;
    return result;
}

// Compute orthonormal basis for converting from tanget/shading space to world space.
void computeBasisVectors(const float3 N, out float3 S, out float3 T)
{
    // Branchless select non-degenerate T.
    T = cross(N, float3(0.0, 1.0, 0.0));
    T = lerp(cross(N, float3(1.0, 0.0, 0.0)), T, step(Epsilon, dot(T, T)));

    T = normalize(T);
    S = normalize(cross(N, T));
}

// Derived from GGX example in:
// http://blog.selfshadow.com/publications/s2013-shading-course/karis/s2013_pbs_epic_notes_v2.pdf
// Image Based Lighting.
//
float3 importanceSampleDiffuse(float2 Xi)
{
    float CosTheta = 1.0 - Xi.y;
    float SinTheta = sqrt(1.0 - CosTheta * CosTheta);
    float Phi = 2 * PI * Xi.x;

    float3 H;
    H.x = SinTheta * cos(Phi);
    H.y = SinTheta * sin(Phi);
    H.z = CosTheta;

    return H;
}

float3 tangentToWorld(const float3 v, const float3 N, const float3 S, const float3 T)
{
    return S * v.x + T * v.y + N * v.z;
}

float3 ImportanceSample(float3 N, float3 MaxRgb)
{
    float3 V = N;

    float4 result = float4(0, 0, 0, 0);

    uint sampleId = 0;
    uint sampleCount = 2048;

    float cubeWidth = 0;
    float cubeHeight = 0;
    float level = 0;

    g_CubeMap.GetDimensions(0, cubeWidth, cubeHeight, level);

    float3 UpVector = abs(N.z) < 0.999 ? float3(0, 0, 1) : float3(1, 0, 0);
    //float3 UpVector = float3(0.f, 1.f, 0.f);
    float3 TangentX = normalize(cross(UpVector, N));
    float3 TangentY = cross(N, TangentX);

    for (uint i = 0; i < sampleCount; i++)
    {
        float2 Xi = Hammersley(i, sampleCount);
        float3 H = tangentToWorld(importanceSampleDiffuse(Xi), N, TangentX, TangentY);
        float3 L = normalize(2 * dot(V, H) * H - V);

        float NoL = saturate(dot(N, L));
        if (NoL > 0.0)
        {
            // Compute Lod using inverse solid angle and pdf.
            // From Chapter 20.4 Mipmap filtered samples in GPU Gems 3.
            // http://http.developer.nvidia.com/GPUGems3/gpugems3_ch20.html
            float pdf = max(0.0, dot(N, L) * INV_PI);

            float solidAngleTexel = 4 * PI / (6 * cubeWidth * cubeWidth);
            float solidAngleSample = 1.0f / (pdf * asfloat(sampleCount));
            float lod = 0.5 * log2((float) (solidAngleSample / solidAngleTexel));

            float3 diffuseSample = rescaleHDR(g_CubeMap.SampleLevel(g_samLinear, H, lod).rgb, MaxRgb);
            result = sumDiffuse(diffuseSample, NoL, result);
        }
    }

    if (result.w == 0)
        return result.xyz;
    else
        return (result.xyz / result.w);
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

//ver final. mipmapFiltered
float4 PSMain(VertexOut pin)
    : SV_Target
{
    //face와 텍스쳐 uv를 큐브맵에서의 좌표

    // x, y 다른 기준으로 -1 ~ 1 사이로 돌립니다.
    float2 uv = float2(pin.Texcoord0.x, 1.f - pin.Texcoord0.y);
    uv = uv * 2.f - float2(1.f, 1.f);

    float3 maxRgb = float3(0.f, 0.f, 0.f);
    
    //큐브맵의서의 좌표로.
    float3 scan = uvToXYZ(face, uv);
    float3 normal = normalize(scan);

    float4 sampledColor = float4(0, 0, 0, 1);

    // Sample source cubemap at specified mip.
    float3 importanceSampled = ImportanceSample(normal, maxRgb);

    sampledColor.xyz = importanceSampled.xyz;

    return sampledColor;
}

//ver2 고정정 위치에 따른 샘플링
// float4 PSMain(VertexOut pin)
//     : SV_Target
// {
//     // face와 텍스쳐 uv를 큐브맵에서의 좌표

//     // x, y 다른 기준으로 -1 ~ 1 사이로 돌립니다.
//     float2 uv = float2(pin.Texcoord0.x, 1.f - pin.Texcoord0.y);
//     uv = uv * 2.f - float2(1.f, 1.f);

//     // 큐브맵의서의 좌표로.
//     float3 scan = uvToXYZ(face, uv);
//     float3 normal = normalize(scan);

//     float3 up = float3(0.f, 1.f, 0.f);
//     float3 right = normalize(cross(up, normal));
//      up = normalize(cross(normal, right));

//      float sampleDelta = 0.025;
//      float nrSamples = 0.0;

//      float3 irradiance = float3(0.f, 0.f, 0.f);

//      for (float phi = 0.0; phi < 2.0 * PI; phi += sampleDelta)
//      {
//         for (float theta = 0.0; theta < 0.5 * PI; theta += sampleDelta)
//         {
//             // spherical to cartesian (in tangent space)
//             float3 tangentSample = float3(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));
//             // tangent space to world
//             float3 sampleVec = tangentSample.x * right + tangentSample.y * up + tangentSample.z * normal;

//             irradiance += g_CubeMap.Sample(g_samLinear, sampleVec).rgb * cos(theta) * sin(theta);
//             nrSamples++;
//         }
//      }
//      irradiance = PI * irradiance * (1.0 / float(nrSamples));

//      return float4(irradiance, 1.f); 
// }

//ver 3 중요 샘플링
// float4 PSMain(VertexOut pin)
//     : SV_Target
// {
//     // face와 텍스쳐 uv를 큐브맵에서의 좌표

//     // x, y 다른 기준으로 -1 ~ 1 사이로 돌립니다.
//     float2 uv = float2(pin.Texcoord0.x, 1.f - pin.Texcoord0.y);
//     uv = uv * 2.f - float2(1.f, 1.f);

//     // 큐브맵의서의 좌표로.
//     float3 scan = uvToXYZ(face, uv);
//     float3 normal = normalize(scan);

//     float3 up = float3(0.f, 1.f, 0.f);
//     float3 right = normalize(cross(up, normal));
//     up = normalize(cross(normal, right));

//     float3 irradiance = float3(0.f, 0.f, 0.f);
//     float totalWeight = 0.f;

//     const uint SAMPLE_COUNT = 4096u;
//     for (uint i = 0u; i < SAMPLE_COUNT; ++i)
//     {
//         float3 R = normal;
//         float3 V = R;

//         float2 Xi = Hammersley(i, SAMPLE_COUNT);
//         float phi = 2.0 * PI * Xi.x;
//         float cosTheta = sqrt((1.0f - Xi.y) / (Xi.y));
//         float sinTheta = sqrt(1.0f - cosTheta * cosTheta);

//         float3 H;
//         H.x = cos(phi) * sinTheta;
//         H.y = sin(phi) * sinTheta;
//         H.z = cosTheta;

//         // from tangent-space vector to world-space sample vector
//         float3 up = abs(normal.z) < 0.999 ? float3(0.0, 0.0, 1.0) : float3(1.0, 0.0, 0.0);
//         float3 tangent = normalize(cross(up, normal));
//         float3 bitangent = cross(normal, tangent);

//         float3 sampleVec = tangent * H.x + bitangent * H.y + normal * H.z;
//         sampleVec = normalize(sampleVec);

//         float3 L = normalize(2.0 * dot(V, H) * H - V);

//         float NdotL = max(dot(normal, L), 0.0);
//         if (NdotL > 0.0)
//         {
//             irradiance += g_CubeMap.Sample(g_samLinear, L).rgb * NdotL;
//             totalWeight += NdotL;
//         }
//     }

//     irradiance /= totalWeight;

//     return float4(irradiance, 1.f);
// }