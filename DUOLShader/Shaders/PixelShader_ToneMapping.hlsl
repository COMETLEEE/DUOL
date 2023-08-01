
cbuffer TonemapConstants : register(b2)
{
	float _exposure;
	float3 _pad;
};

struct VS_OUT
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
};

Texture2D TextureFinal : register(t0);

SamplerState SamClamp : register(s0);      // ���÷� (Clamp)

float4 ReinhardTonemap(float4 color)
{
	color = color / (1 + color);

	color = pow(color, (1 / 2.2f));

	return color;
}

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

float4 Uncharted2(float3 texColor)
{
    float W = 11.2;

    // float ExposureBias = 0.5f;           // 어둡
    float ExposureBias = _exposure;           // 밝음
    float3 curr = Uncharted2Kernel(ExposureBias * texColor);

    float3 whiteScale = 1.0f / Uncharted2Kernel(W);
    float3 color = curr * whiteScale;

    float3 retColor = pow(color, 1 / 2.2);
    return float4(retColor, 1);
}

float4 PSMain(VS_OUT input) : SV_TARGET
{
	float4 color = (float4) 0.f;

	color = TextureFinal.Sample(SamClamp, input.uv);

	  color *= _exposure;
	  color = ReinhardTonemap(color);
	  color.a = 1.f;

   //color = Uncharted2(color.xyz);
	
	return color;
}