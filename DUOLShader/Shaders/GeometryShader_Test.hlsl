#include "ConstantBuffer.hlsli"

//--------------------------------------------------------------------------------------
// Globals
//--------------------------------------------------------------------------------------
//// PerObject
cbuffer cbPerObject : register(b1)
{
};

//--------------------------------------------------------------------------------------
// Input / Output structures
//--------------------------------------------------------------------------------------

struct GS_OUTPUT
{
    float4 PosH : SV_POSITION;
    float3 PosW : POSITION;
    float3 Tangent : TANGENT;
    float3 Normal : NORMAL;
    float2 Texcoord0 : TEXCOORD0;
};

//--------------------------------------------------------------------------------------
// Geometry Shader
//--------------------------------------------------------------------------------------
[maxvertexcount(9)]
void GSMain(triangle GS_OUTPUT input[3], inout TriangleStream<GS_OUTPUT> stream)
{
    GS_OUTPUT output;
    
    for (int i = 0; i < 3; ++i)
    {
		// Generate 3 vertices for each triangle
        for (int j = 0; j < 3; ++j)
        {
            output = input[j];
            output.PosH.x += (float(i) - 1.0) * 2.0;
            stream.Append(output);
        }

        stream.RestartStrip();
    }
}