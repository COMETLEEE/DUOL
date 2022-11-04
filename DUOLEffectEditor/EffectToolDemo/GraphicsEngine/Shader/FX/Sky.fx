//=============================================================================
// Sky.fx by Frank Luna (C) 2011 All Rights Reserved.
//
// Effect used to shade sky dome.
//=============================================================================

cbuffer cbPerFrame
{
    float4x4 gWorld; // 월드 메트릭스!!
	float4x4 gWorldViewProj;
};
 
// Nonnumeric values cannot be added to a cbuffer.
TextureCube gCubeMap;

SamplerState samTriLinearSam
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

struct VertexIn
{
	float3 PosL : POSITION;
};

struct VertexOut
{
	float4 PosH : SV_POSITION;
    float3 PosL : POSITION;
    float4 PosW : POSITION1;
};
struct PSOut
{
    float4 Depth : SV_Target0;
    float4 Normal : SV_Target1;
    float4 Position : SV_Target2;
    float4 Albedo : SV_Target3;
};
VertexOut VS(VertexIn vin)
{
	VertexOut vout;
	
	// Set z = w so that z/w = 1 (i.e., skydome always on far plane).
    vout.PosW = mul(float4(vin.PosL, 1.0f), gWorld);
	vout.PosH = mul(float4(vin.PosL, 1.0f), gWorldViewProj).xyzw;
	
	// Use local vertex position as cubemap lookup vector.
	vout.PosL = vin.PosL;
	
	return vout;
}

PSOut PS(VertexOut pin) : SV_Target
{
    PSOut _PSOut;
    _PSOut.Depth = float4(0.0f, 0.0f, 0.0f, 0.0f);
    _PSOut.Normal = float4(0.0f, 0.0f, 0.0f, 0.0f);
    _PSOut.Position = pin.PosW;
    _PSOut.Albedo = gCubeMap.Sample(samTriLinearSam, pin.PosL);
    return _PSOut;
}

RasterizerState NoCull
{
    CullMode = None;
};

DepthStencilState LessEqualDSS
{
	// Make sure the depth function is LESS_EQUAL and not just LESS.  
	// Otherwise, the normalized depth values at z = 1 (NDC) will 
	// fail the depth test if the depth buffer was cleared to 1.
    DepthFunc = LESS_EQUAL;
};

technique11 SkyTech
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, PS() ) );
        
        SetRasterizerState(NoCull);
        SetDepthStencilState(LessEqualDSS, 0);
    }
}
