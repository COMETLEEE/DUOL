Texture2D gDepthBuffer; // ��ƼŬ�� �׸� �� �տ� ������Ʈ�� ������ �׸��� �ʱ� ���ؼ� �������۸� �����Ѵ�.��

cbuffer cbPerFrame
{
	float3 gEyePosW; // ī�޶��� ��ǥ
	float3 gEmitPosW; // ����� ��ǥ
	float3 gEmitDirW; // ����� ����
	float gGameTime; // ���� �ð�
	float gTimeStep; // 1�����Ӵ� �ð�
	float4x4 gViewProj;
	float2 gScreenXY;
};

cbuffer ParticleData
{
	float gStartSpeed; // ��ƼŬ ��� �ӵ�.
	float gLifeTime; // ��ƼŬ�� �󸶳� ���������?

	uint gEmissiveCount; // �ѹ��� �����ϴ� ��ƼŬ�� ��.
	float gEmissiveTime; // ���� �ֱ�

	float2 gStartSize;
};

cbuffer cbFixed
{
	// ���ӵ�
	float3 gAccelW = { 0.0f, 7.8f, 0.0f };

	float2 gQuadTexC[4] =
	{
		float2(0.0f, 1.0f),
		float2(1.0f, 1.0f),
		float2(0.0f, 0.0f),
		float2(1.0f, 0.0f)
	};
};

Texture2DArray gTexArray;
// HLSL���� �����Լ��� ����Ǿ� ���� �ʾƼ� ���� �ؽ�ó�� ����� ���� ����
Texture1D gRandomTex;

SamplerState samLinear
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = WRAP;
	AddressV = WRAP;
};

DepthStencilState DisableDepth
{
	DepthEnable = FALSE;
	DepthWriteMask = ZERO;
};

DepthStencilState NoDepthWrites
{
	DepthEnable = TRUE;
	DepthWriteMask = ZERO;
};

BlendState AdditiveBlending // ���� ȥ��
{
	AlphaToCoverageEnable = FALSE;
	BlendEnable[0] = TRUE;

	//ȥ�� ���İ� ���õ� �Ű�����
	SrcBlend = SRC_ALPHA;
	DestBlend = ONE;
	BlendOp = ADD;
	//ȥ�� ���İ� ���õ� �Ű�����

	SrcBlendAlpha = ZERO;
	DestBlendAlpha = ZERO;
	BlendOpAlpha = ADD;
	RenderTargetWriteMask[0] = 0x0F;
};


float3 RandUnitVec3(float offset)
{
	float u = (gGameTime + offset);

	// hlsl �� �����Լ��� ����Ǿ� ���� �ʾ� ���� �ؽ��ĸ� ���� ���� ����.
	float3 v = gRandomTex.SampleLevel(samLinear, u, 0).xyz;

	return normalize(v);
}

//***********************************************
// STREAM-OUT TECH                              *
//***********************************************

#define PT_EMITTER 0
#define PT_FLARE 1

struct Particle
{
	float3 InitialPosW : POSITION;
	float3 InitialVelW : VELOCITY;
	float2 SizeW       : SIZE;
	float Age : AGE;
	uint Type          : TYPE;
};

Particle StreamOutVS(Particle vin)
{
	return vin;
}

[maxvertexcount(2)]
void StreamOutGS(point Particle gin[1],
	inout PointStream<Particle> ptStream)
{
	gin[0].Age += gTimeStep;

	if (gin[0].Type == PT_EMITTER) // 0�̶�� �����.
	{
		// ���� �ð����� ����
		if (gin[0].Age > gEmissiveTime)
		{
			float3 vRandom = RandUnitVec3(0.0f);
			vRandom.x *= 0.5f;
			vRandom.z *= 0.5f;

			Particle p;
			p.InitialPosW = gEmitPosW.xyz;
			p.InitialVelW = gStartSpeed * vRandom;
			p.SizeW = float2(3.0f, 3.0f);
			p.Age = 0.0f;
			p.Type = PT_FLARE;

			ptStream.Append(p);
			// ���� �ð����� ���ο� ���ؽ� ����.
			// �ð� ����
			gin[0].Age = 0.0f;
		}

		// �׻� ������ ������Ų��.
		ptStream.Append(gin[0]);
	}
	else
	{
		// ��ƼŬ�� �����ð�
		if (gin[0].Age <= gLifeTime)
			ptStream.Append(gin[0]);
	}
}

GeometryShader gsStreamOut = ConstructGSWithSO(
	CompileShader(gs_5_0, StreamOutGS()),
	"POSITION.xyz; VELOCITY.xyz; SIZE.xy; AGE.x; TYPE.x");

technique11 StreamOutTech
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, StreamOutVS()));
		SetGeometryShader(gsStreamOut);

		// �ȼ����̴��� ����.
		SetPixelShader(NULL);

		// ��Ʈ�� �ƿ��� �� �� �������۸� �����Ѵ�.
		SetDepthStencilState(DisableDepth, 0);
	}
}

struct VertexOut
{
	float3 PosW  : POSITION;
	float2 SizeW : SIZE;
	float4 Color : COLOR;
	uint   Type  : TYPE;
};

VertexOut DrawVS(Particle vin)
{
	VertexOut vout;

	float t = vin.Age;

	// ���ӵ� ����
	vout.PosW = 0.5f * t * t * gAccelW + t * vin.InitialVelW + vin.InitialPosW;

	// �ð��� �������� ���� ��������
	float opacity = 1.0f - smoothstep(0.0f, 1.0f, t / 1.0f);
	vout.Color = float4(1.0f, 1.0f, 1.0f, opacity);

	vout.SizeW = vin.SizeW;
	vout.Type = vin.Type;

	return vout;
}

struct GeoOut
{
	float4 PosH  : SV_Position;
	float4 Color : COLOR;
	float2 Pos  : TEXCOORD1; // NDC������ ��ǥ ��.
	float2 Tex   : TEXCOORD2;
};

// �ϳ��� ���� 4���� Ȯ���ؼ� �ؽ�ó�� ������ ����Ѵ�..!
[maxvertexcount(4)]
void DrawGS(point VertexOut gin[1],
	inout TriangleStream<GeoOut> triStream)
{
	// ������ �Ѿ��
	if (gin[0].Type != PT_EMITTER)
	{
		//
		// ī�޶� �������� �迧
		//
		float3 look = normalize(gEyePosW.xyz - gin[0].PosW);
		float3 right = normalize(cross(float3(0, 1, 0), look));
		float3 up = cross(look, right);

		float halfWidth = 0.5f * gStartSize.x;
		float halfHeight = 0.5f * gStartSize.y;

		float4 v[4];
		v[0] = float4(gin[0].PosW + halfWidth * right - halfHeight * up, 1.0f);
		v[1] = float4(gin[0].PosW + halfWidth * right + halfHeight * up, 1.0f);
		v[2] = float4(gin[0].PosW - halfWidth * right - halfHeight * up, 1.0f);
		v[3] = float4(gin[0].PosW - halfWidth * right + halfHeight * up, 1.0f);


		GeoOut gout;
		[unroll]
		for (int i = 0; i < 4; ++i)
		{
			gout.PosH = mul(v[i], gViewProj);

			gout.Pos = gout.PosH / gout.PosH.w;


			gout.Tex = gQuadTexC[i];
			gout.Color = gin[0].Color;
			triStream.Append(gout);
		}
	}
}

float4 DrawPS(GeoOut pin) : SV_Target
{

	float4 temp = pin.PosH.xyzw;
	float2 posTexCoord = pin.Pos.xy;

	posTexCoord = posTexCoord * 0.5f + 0.5f;
	posTexCoord.y = 1 - posTexCoord.y;

	posTexCoord = float2(gScreenXY.x, gScreenXY.y);
	posTexCoord= pin.PosH.xy / gScreenXY;
	float4 depth = gDepthBuffer.Sample(samLinear, posTexCoord);

	if ((depth.x >= 0.001f) && (depth.x - pin.PosH.z < 0.001f))
	{
		clip(-1.f);
	}

	//if (depth.x >= 0.001f)
	//{
	//	clip(depth.x - pin.PosH.z);
	//}
	return gTexArray.Sample(samLinear, float3(pin.Tex, 0)) * pin.Color;
}

technique11 DrawTech
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, DrawVS()));
		SetGeometryShader(CompileShader(gs_5_0, DrawGS()));
		SetPixelShader(CompileShader(ps_5_0, DrawPS()));

		SetBlendState(AdditiveBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);
		SetDepthStencilState(NoDepthWrites, 0);
	}
}