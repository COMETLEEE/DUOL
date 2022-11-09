Texture2D gDepthBuffer; // 파티클을 그릴 때 앞에 오브젝트가 있으면 그리지 않기 위해서 뎁스버퍼를 참조한다.ㄴ

cbuffer cbPerFrame
{
	float3 gEyePosW; // 카메라의 좌표
	float3 gEmitPosW; // 방출기 좌표
	float3 gEmitDirW; // 방출기 방향
	float gGameTime; // 게임 시간
	float gTimeStep; // 1프레임당 시간
	float4x4 gViewProj;
	float2 gScreenXY;
};

cbuffer ParticleData
{
	float gStartSpeed; // 파티클 출발 속도.
	float gLifeTime; // 파티클이 얼마나 살아있을까?

	uint gEmissiveCount; // 한번에 방출하는 파티클의 수.
	float gEmissiveTime; // 방출 주기

	float2 gStartSize;
};

cbuffer cbFixed
{
	// 가속도
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
// HLSL에는 랜덤함수가 내장되어 있지 않아서 랜덤 텍스처를 만들어 랜덤 구현
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

BlendState AdditiveBlending // 가산 혼합
{
	AlphaToCoverageEnable = FALSE;
	BlendEnable[0] = TRUE;

	//혼합 공식과 관련된 매개변수
	SrcBlend = SRC_ALPHA;
	DestBlend = ONE;
	BlendOp = ADD;
	//혼합 공식과 관련된 매개변수

	SrcBlendAlpha = ZERO;
	DestBlendAlpha = ZERO;
	BlendOpAlpha = ADD;
	RenderTargetWriteMask[0] = 0x0F;
};


float3 RandUnitVec3(float offset)
{
	float u = (gGameTime + offset);

	// hlsl 에 랜덤함수가 내장되어 있지 않아 랜덤 텍스쳐를 통해 랜덤 구현.
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

	if (gin[0].Type == PT_EMITTER) // 0이라면 방출기.
	{
		// 일정 시간마다 방출
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
			// 일정 시간마다 새로운 버텍스 생성.
			// 시간 리셋
			gin[0].Age = 0.0f;
		}

		// 항상 방출기는 유지시킨다.
		ptStream.Append(gin[0]);
	}
	else
	{
		// 파티클의 생존시간
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

		// 픽셀셰이더를 끈다.
		SetPixelShader(NULL);

		// 스트림 아웃을 할 땐 뎁스버퍼를 꺼야한다.
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

	// 가속도 공식
	vout.PosW = 0.5f * t * t * gAccelW + t * vin.InitialVelW + vin.InitialPosW;

	// 시간이 지날수록 색이 옅어지게
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
	float2 Pos  : TEXCOORD1; // NDC공간의 좌표 값.
	float2 Tex   : TEXCOORD2;
};

// 하나의 점을 4개로 확장해서 텍스처를 입혀서 출력한다..!
[maxvertexcount(4)]
void DrawGS(point VertexOut gin[1],
	inout TriangleStream<GeoOut> triStream)
{
	// 방출기는 넘어가자
	if (gin[0].Type != PT_EMITTER)
	{
		//
		// 카메라 방향으로 룩엣
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