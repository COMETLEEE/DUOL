#include "ConstantBuffer.hlsli"

Texture2D gTextureMap : register(t0); // 출력하기 위함

float4 psEdgeDetect(float2 texCoord : TEXCOORD0) : COLOR0
{
    //float mask[25] =
    //{
    //    -1, -1, -1, -1, -1,
    //    -1, -1, -1, -1, -1,
    //    -1, -1, 24, -1, -1,
    //    -1, -1, -1, -1, -1,
    //    -1, -1, -1, -1, -1
    //}; // Laplacian Filter
           
    float mask[9] =
    {
        -1, -1, -1,
        -1, 8, -1,
        -1, -1, -1
    }; // Laplacian Filter
           
    
    float coord[3] = { -1, 0, +1 };

    float4 c0 = 0;

    for (int i = 0; i < 9; i++)
        c0 += mask[i] * gTextureMap.Sample(samAnisotropic, texCoord + float2(coord[i % 3] / gScreenXY.x, coord[i / 3] / gScreenXY.y));

    return clamp(c0, 0, 1.0f);
}

float4 Blur(float2 texCoord : TEXCOORD0) : COLOR0
{
    //float weights[81] =
    //{
    //    0.013519, 0.013519, 0.013519, 0.013519, 0.013519, 0.013519, 0.013519, 0.013519, 0.013519,
    //    0.013519, 0.047662, 0.047662, 0.047662, 0.047662, 0.047662, 0.047662, 0.047662, 0.013519,
    //    0.013519, 0.047662, 0.117230, 0.117230, 0.117230, 0.117230, 0.117230, 0.047662, 0.013519,
    //    0.013519, 0.047662, 0.117230, 0.201167, 0.201167, 0.201167, 0.117230, 0.047662, 0.013519,
    //    0.013519, 0.047662, 0.117230, 0.201167, 0.240841, 0.201167, 0.117230, 0.047662, 0.013519,
    //    0.013519, 0.047662, 0.117230, 0.201167, 0.201167, 0.201167, 0.117230, 0.047662, 0.013519,
    //    0.013519, 0.047662, 0.117230, 0.117230, 0.117230, 0.117230, 0.117230, 0.047662, 0.013519,
    //    0.013519, 0.047662, 0.047662, 0.047662, 0.047662, 0.047662, 0.047662, 0.047662, 0.013519,
    //    0.013519, 0.013519, 0.013519, 0.013519, 0.013519, 0.013519, 0.013519, 0.013519, 0.013519
    //};
    float weights[25] =
    {
        0.117230, 0.117230, 0.117230, 0.117230, 0.117230,
         0.117230, 0.214686, 0.214686, 0.214686, 0.117230,
         0.117230, 0.214686, 0.336165, 0.214686, 0.117230,
         0.117230, 0.214686, 0.214686, 0.214686, 0.117230,
         0.117230, 0.117230, 0.117230, 0.117230, 0.117230
    };
    float coord[5] = { -2, -1, 0, +1, +2 };

    float4 c0 = 0;
    
    [unroll]
    for (int i = 0; i < 25; i++)
        c0 += weights[i] * gTextureMap.Sample(samAnisotropic, texCoord + float2(coord[i % 5] / gScreenXY.x, coord[i / 5] / gScreenXY.y));
    
    c0.w /= 5;
    return clamp(c0, 0, 1.0f);
}

struct VS_OUT
{
    float4 pos : SV_POSITION;
    float2 uv : UV;
};
// 포스트 프로세싱
float4 PS_OutLine(VS_OUT input) : SV_Target
{

    float4 color = psEdgeDetect(input.uv);

    return color;
}

// 포스트 프로세싱
float4 PS_DownScaling(VS_OUT input) : SV_Target
{
    float4 color = gTextureMap.Sample(samAnisotropic, input.uv);

    return color;
}
float4 PS_Blur(VS_OUT input) : SV_Target
{
    float4 color = Blur(input.uv);

    return color;
}