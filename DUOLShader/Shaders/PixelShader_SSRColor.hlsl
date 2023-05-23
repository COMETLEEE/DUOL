// By Morgan McGuire and Michael Mara at Williams College 2014
// Released as open source under the BSD 2-Clause License
// http://opensource.org/licenses/BSD-2-Clause
//
// Copyright (c) 2014, Morgan McGuire and Michael Mara
// All rights reserved.
//
// From McGuire and Mara, Efficient GPU Screen-Space Ray Tracing,
// Journal of Computer Graphics Techniques, 2014
//
// This software is open source under the "BSD 2-clause license":
//
// Redistribution and use in source and binary forms, with or
// without modification, are permitted provided that the following
// conditions are met:
//
// 1. Redistributions of source code must retain the above
// copyright notice, this list of conditions and the following
// disclaimer.
//
// 2. Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following
// disclaimer in the documentation and/or other materials provided
// with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
// CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
// INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
// USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
// AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
// LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
// IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
// THE POSSIBILITY OF SUCH DAMAGE.
/**
 * The ray tracing step of the SSLR implementation.
 * Modified version of the work stated above.
 */

#include "ConstantBuffer.hlsli"

#ifdef SSRColor
//--------------------------------------------------------------------------------------
// Globals
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Textures and Samplers
//--------------------------------------------------------------------------------------

Texture2D g_UV : register(t0);
Texture2D g_Color : register(t1);

SamplerState g_samLinear : register(s0);

//--------------------------------------------------------------------------------------
// Input / Output structures
//--------------------------------------------------------------------------------------

struct PS_IN
{
    float4 PosH : SV_POSITION;
    float2 Texcoord0 : TEXCOORD0;
};

// struct PS_OUT
// {
//     float4 Color : SV_Target0;
//     float4 ColorBlur : SV_Target1;
// };

//--------------------------------------------------------------------------------------
// Functions
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------

float4 PSMain(PS_IN input)
    : SV_TARGET
{
    int size = 3;
    float separation = 2.0;

    int3 uid = int3(input.PosH.xy, 0);

    float4 uv = g_UV.Load(uid);
    float4 color = g_Color.Sample(g_samLinear, uv.xy);


    // visibility <= 0
    // 평균적인 주위의 값을 가져와서 혹시모를 구멍들을 제거합니다.
    // if (uv.b <= 0.0)
    // {
    //     uv = float4(0.f, 0.f, 0.f, 0.f);
    //     float count = 0.0;

    //     for (int i = -size; i <= size; ++i)
    //     {
    //         for (int j = -size; j <= size; ++j)
    //         {
    //             uv += g_UV.Sample(g_samLinear, ((float2(i, j) * separation) / g_ScreenXY) + input.Texcoord0);
    //             count += 1.0;
    //         }
    //     }

    //     uv.xyz /= count;
    // }

    //이래도 값이 낮으면... 컷!
    if (uv.b <= 0.0)
    {
        clip(-1);
    }

    return color;
}

#elif colorOut

//--------------------------------------------------------------------------------------
// Globals
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Textures and Samplers
//--------------------------------------------------------------------------------------

Texture2D g_Color : register(t0);
Texture2D g_ColorBlur : register(t1);
Texture2D g_Specular : register(t2);

SamplerState g_samLinear : register(s0);

//--------------------------------------------------------------------------------------
// Input / Output structures
//--------------------------------------------------------------------------------------

struct PS_IN
{
    float4 PosH : SV_POSITION;
    float2 Texcoord0 : TEXCOORD0;
};

//--------------------------------------------------------------------------------------
// Functions
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------

float4 PSMain(PS_IN input)
    : SV_TARGET
{
    float4 color = g_Color.Sample(g_samLinear, input.Texcoord0);
    float4 colorBlur = g_ColorBlur.Sample(g_samLinear, input.Texcoord0);
    float4 specular = g_Specular.Sample(g_samLinear, input.Texcoord0);

    float specularAmount = dot(specular.xyz, float3(1.f, 1.f, 1.f)) / 3;

    if (specularAmount <= 0)
    {
        clip(-1);
    }

    float4 retColor = float4(lerp(color.xyz, colorBlur.xyz, specular.w) * specularAmount, 1.f);

    return retColor;
}

#else

//--------------------------------------------------------------------------------------
// Globals
//--------------------------------------------------------------------------------------
cbuffer cbperObject : register(b2)
{
    float cb_stride;
    // Step in horizontal or vertical pixels between samples. This is a float
    // because integer math is slow on GPUs, but should be set to an integer >= 1.
    float cb_maxSteps;    // Maximum number of iterations. Higher gives better images but may be slow.
    float cb_maxDistance; // Maximum camera-space distance to trace before returning a miss.
    float cb_strideZCutoff;
    // More distant pixels are smaller in screen space. This value tells at what point to
    // start relaxing the stride to give higher quality reflections for objects far from
    // the camera.

    float cb_numMips;       // the number of mip levels in the convolved color buffer
    float cb_fadeStart;     // determines where to start screen edge fading of effect
    float cb_fadeEnd;       // determines where to end screen edge fading of effect
    float cb_sslr_padding0; // padding for alignment

    float cb_zThickness; // thickness to ascribe to each pixel in the depth buffer
};
//--------------------------------------------------------------------------------------
// Textures and Samplers
//--------------------------------------------------------------------------------------

SamplerState g_samLinear : register(s0);
SamplerState sampTrilinearClamp : register(s1);

Texture2D g_Depth : register(t0); // scene depth buffer used in ray tracing step

Texture2D g_SSRUV : register(t1); // ray-tracing buffer

Texture2D g_Normal : register(t2);     // normal buffer - from g-buffer

Texture2D g_MetallicRoughnessAOSpecular : register(t3); // specular buffer - from g-buffer (rgb = ior, a = roughness)

Texture2D g_LightOutput : register(t4); // indirect specular light buffer used for fallback

Texture2D g_PosW : register(t5);

Texture2D g_SSRConvColors : register(t6); // convolved color buffer - all mip levels

//--------------------------------------------------------------------------------------
// Input / Output structures
//--------------------------------------------------------------------------------------

struct PS_IN
{
    float4 PosH : SV_POSITION;
    float2 Texcoord0 : TEXCOORD0;
};

float linearizeDepth(float depth, float near, float far)
{
    return near * far / (far - depth * (far - near));
}

//--------------------------------------------------------------------------------------
// Functions
//--------------------------------------------------------------------------------------

#define CNST_MAX_SPECULAR_EXP 64

///////////////////////////////////////////////////////////////////////////////////////
// Cone tracing methods
///////////////////////////////////////////////////////////////////////////////////////
float specularPowerToConeAngle(float specularPower)
{
    // based on phong distribution model
    if (specularPower >= exp2(CNST_MAX_SPECULAR_EXP))
    {
        return 0.0f;
    }
    const float xi = 0.244f;
    float exponent = 1.0f / (specularPower + 1.0f);
    return acos(pow(xi, exponent));
}

float roughnessToSpecularPower(float r)
{
    return 2 / (pow(r, 4)) - 2;
}

float isoscelesTriangleOpposite(float adjacentLength, float coneTheta)
{
    // simple trig and algebra - soh, cah, toa - tan(theta) = opp/adj, opp = tan(theta) * adj, then multiply * 2.0f for isosceles triangle base
    return 2.0f * tan(coneTheta) * adjacentLength;
}

float isoscelesTriangleInRadius(float a, float h)
{
    float a2 = a * a;
    float fh2 = 4.0f * h * h;
    return (a * (sqrt(a2 + fh2) - a)) / (4.0f * h);
}

float4 coneSampleWeightedColor(float2 samplePos, float mipChannel, float gloss)
{
    float3 sampleColor = g_SSRConvColors.SampleLevel(sampTrilinearClamp, samplePos, mipChannel).rgb;
    //float3 sampleColor = g_LightOutput.Sample(sampTrilinearClamp, samplePos).rgb;

    return float4(sampleColor * gloss, gloss);
}

float isoscelesTriangleNextAdjacent(float adjacentLength, float incircleRadius)
{
    // subtract the diameter of the incircle to get the adjacent side of the next level on the cone
    return adjacentLength - (incircleRadius * 2.0f);
}

float3 viewSpacePositionFromDepth(float2 uv, float depth)
{
    float4 clip;
    clip.xy = uv;
    clip.xy -= 0.5f;
    clip.xy *= float2(2.0f, -2.f);
    clip.z = depth;
    clip.w = 1;

    float4 viewSpace = mul(clip, g_Camera.g_InverseProjectionMatrix);
    return viewSpace.xyz / viewSpace.w;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------

float4 PSMain(PS_IN input) : SV_TARGET
{
    int3 loadPixel = int3(input.PosH.xy, 0);

    // get screen-space ray intersection point
    float4 raySS = g_SSRUV.Load(loadPixel).xyzw;
    float3 fallbackColor = g_LightOutput.Load(loadPixel).rgb;

    if (raySS.w <= 0.0f) // either means no hit or the ray faces back towards the camera
    {
       //no data for this point - a fallback like localized environment maps should be used
       return float4(fallbackColor, 1.0f);
    }

    float depth = g_Depth.Load(loadPixel).r;

    float3 positionSS = float3(input.Texcoord0, depth);
    float3 viewRay = mul(float4(g_PosW.Load(loadPixel).xyz, 1.f), g_Camera.g_ViewMatrix).xyz;

    float linearDepth = linearizeDepth(depth, g_Camera.g_CameraNear, g_Camera.g_CameraFar);
    float3 positionVS = normalize(viewRay) * linearDepth;

    // since calculations are in view-space, we can just normalize the position to point at it
    float3 toPositionVS = normalize(positionVS);
    float3 normalVS = mul(g_Normal.Load(loadPixel).xyz, (float3x3)g_Camera.g_ViewMatrix);

    // get specular power from roughness
    float4 specularAll = g_MetallicRoughnessAOSpecular.Load(loadPixel);
    //float4 specularAll = float4(0.f,0.f,0.f,0.f);
    float gloss = 1.0f - specularAll.y;
    float specularPower = roughnessToSpecularPower(specularAll.y);
    
    // convert to cone angle (maximum extent of the specular lobe aperture)
    // only want half the full cone angle since we're slicing the isosceles triangle in half to get a right triangle
    float coneTheta = specularPowerToConeAngle(specularPower) * 0.5f;

    // P1 = positionSS, P2 = raySS, adjacent length = ||P2 - P1||
    float2 deltaP = raySS.xy - positionSS.xy;
    float adjacentLength = length(deltaP);
    float2 adjacentUnit = normalize(deltaP);

    float4 totalColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float remainingAlpha = 1.0f;
    float maxMipLevel = (float)cb_numMips - 1.0f;
    float glossMult = gloss;

    // cone-tracing using an isosceles triangle to approximate a cone in screen space
    for (int i = 0; i < 14; ++i)
    {
        // intersection length is the adjacent side, get the opposite side using trig
        float oppositeLength = isoscelesTriangleOpposite(adjacentLength, coneTheta);

        // calculate in-radius of the isosceles triangle
        float incircleSize = isoscelesTriangleInRadius(oppositeLength, adjacentLength);

        // get the sample position in screen space
        float2 samplePos = positionSS.xy + adjacentUnit * (adjacentLength - incircleSize);

        // convert the in-radius into screen size then check what power N to raise 2 to reach it - that power N becomes mip level to sample from
        float mipChannel = clamp(log2(incircleSize * max(g_ScreenXY.x, g_ScreenXY.y)), 0.0f, maxMipLevel);

        /*
         * Read color and accumulate it using trilinear filtering and weight it.
         * Uses pre-convolved image (color buffer) and glossiness to weigh color contributions.
         * Visibility is accumulated in the alpha channel. Break if visibility is 100% or greater (>= 1.0f).
         */
        float4 newColor = coneSampleWeightedColor(samplePos, mipChannel, glossMult);

        remainingAlpha -= newColor.a;
        
        if (remainingAlpha < 0.0f)
        {
            newColor.rgb *= (1.0f - abs(remainingAlpha));
        }
        totalColor += newColor;

        if (totalColor.a >= 1.0f)
        {
            break;
        }

        adjacentLength = isoscelesTriangleNextAdjacent(adjacentLength, incircleSize);
        glossMult *= gloss;
    }

    float3 toEye = -toPositionVS;
    specularAll.rgb = float3(0.04f, 0.04f, 0.04f);
    float3 specular = fresnelSchlick(specularAll.rgb, abs(dot(normalize(normalVS), toEye))) * PBR_INV_PI;

    // fade rays close to screen edge
    float2 boundary = abs(raySS.xy - float2(0.5f, 0.5f)) * 2.0f;
    const float fadeDiffRcp = 1.0f / (cb_fadeEnd - cb_fadeStart);
    float fadeOnBorder = 1.0f - saturate((boundary.x - cb_fadeStart) * fadeDiffRcp);
    fadeOnBorder *= 1.0f - saturate((boundary.y - cb_fadeStart) * fadeDiffRcp);
    fadeOnBorder = smoothstep(0.0f, 1.0f, fadeOnBorder);

    float3 rayHitPositionVS = viewSpacePositionFromDepth(raySS.xy, raySS.z);
    float fadeOnDistance = 1.0f - saturate(distance(rayHitPositionVS, positionVS) / cb_maxDistance);
    // ray tracing steps stores rdotv in w component - always > 0 due to check at start of this method
    float fadeOnPerpendicular = saturate(lerp(0.0f, 1.0f, saturate(raySS.w * 4.0f)));
    float fadeOnRoughness = saturate(lerp(0.0f, 1.0f, gloss * 4.0f));
    float totalFade = fadeOnBorder * fadeOnDistance * fadeOnPerpendicular * fadeOnRoughness * (1.0f - saturate(remainingAlpha));


    float aoFallOff = length((fallbackColor / (fallbackColor + 1)));

    // float4 retColor = float4(lerp(fallbackColor, totalColor.rgb * specular, totalFade), 1.0f);
    float4 retColor = float4(fallbackColor + lerp(float3(0.f, 0.f, 0.f), totalColor.rgb * specular, totalFade), 1.0f);
    return retColor; 
}

#endif 