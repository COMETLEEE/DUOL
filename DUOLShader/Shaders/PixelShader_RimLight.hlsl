#include "ConstantBuffer.hlsli"

//--------------------------------------------------------------------------------------
// Globals
//--------------------------------------------------------------------------------------

struct RimLight
{
    float g_ClipPoint;
    float g_RimDecrease;
    float2 pad;
};

// PerObject
cbuffer cbPerObject : register(b2)
{
    RimLight g_RimLight;
};

//--------------------------------------------------------------------------------------
// Textures and Samplers
//--------------------------------------------------------------------------------------

Texture2D g_Normal : register(t0);
Texture2D g_RimPixel : register(t1);
Texture2D g_World : register(t2);
Texture2D g_Effect : register(t3);

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
    float4 ObjectFlag = g_RimPixel.Load(int3(input.PosH.xy, 0));
    uint flag = asuint(ObjectFlag.w);

    //bool isRimValue = ();

    if (!(flag == (1 << 1)))
     {
         clip(-1);
     }
     float4 Effect = g_Effect.Load(int3(input.PosH.xy, 0));
     float4 color = UnpackingColor(asuint(Effect.x));

    float4 normal = g_Normal.Sample(g_samLinear, input.Texcoord0);
    float4 world = g_World.Sample(g_samLinear, input.Texcoord0);
    if (world.w == 0)
    {
        clip(-1);
    }

    float3 viewDir = normalize(g_Camera.g_CameraPosition - world.xyz);

    float rimValue = max(0.f, dot(normalize(normal.xyz), viewDir));
    
    
    //limValue = smoothstep(1.0f - g_RimLight.g_Stride, 1.0f, 1.f - limValue);
    rimValue = 1.f - saturate(rimValue);
    if(rimValue < g_RimLight.g_ClipPoint)
    {
        clip(-1);
    }

    rimValue = pow(rimValue, g_RimLight.g_RimDecrease);

    return float4(color.xyz * Effect.y , rimValue);
}
