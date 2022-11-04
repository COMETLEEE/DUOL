//***************************************************************************************
// color.fx by Frank Luna (C) 2011 All Rights Reserved.
//
// Transforms and colors geometry.
//***************************************************************************************

cbuffer cbPerObject
{
	float4x4 gWorldViewProj;
};

struct VertexIn
{
	float3 PosL  : POSITION;
	float4 Color : COLOR;
};

struct VertexOut
{
	float4 PosH  : SV_POSITION;
	float4 Color : COLOR;
};

// 이 함수는 어디서 부르는가?
VertexOut VS(VertexIn vin)
{
	VertexOut vout;

	// Transform to homogeneous clip space.
	vout.PosH = mul(float4(vin.PosL, 1.0f), gWorldViewProj);

	// Just pass vertex color into the pixel shader.
	vout.Color = vin.Color;

	/*vout.Color.x = 0;
	vout.Color.y = 1.0f;
	vout.Color.z = 0;
	vout.Color.w = 1.0f;*/

	return vout;
}

float4 PS(VertexOut pin) : SV_Target
{
	//float4 test;
	//test.r = 1.0f;
	//test.g = 0.0f;
	//test.b = 1.0f;
	//test.a = 1.0f;
	//return test;

	//pin.Color.x = pin.PosH.z;
	//pin.Color.y = pin.PosH.z;
	//pin.Color.z = pin.PosH.z;

	//pin.Color.r = 0.0f;
	//pin.Color.g = 0.7f;
	//pin.Color.b = 0.0f;

	return pin.Color;
}

technique11 ColorTech
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}
}
