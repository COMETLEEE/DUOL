
SamplerState Sam_Clamp : register(s0);

Texture2D g_InputTexture:register(t0);

//--------------------------------------------------------------------------------------
// Input / Output structures
//--------------------------------------------------------------------------------------
struct VS_OUTPUT
{
    float4 PosH : SV_POSITION;
    float2 uv : TEXCOORD0;
};

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PSMain(VS_OUTPUT input) : SV_TARGET
{
    float4 color;

    color = g_InputTexture.Sample(Sam_Clamp, input.uv);

	return color;
}