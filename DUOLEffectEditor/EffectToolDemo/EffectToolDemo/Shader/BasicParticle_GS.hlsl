#include "ConstantBuffer.hlsli"

Texture1D gRandomTex; // HLSL에는 랜덤함수가 내장되어 있지 않아서 랜덤 텍스처를 만들어 랜덤 구현

cbuffer cbFixed
{
    float2 gQuadTexC[4] =
    {
        float2(0.0f, 1.0f),
		float2(1.0f, 1.0f),
		float2(0.0f, 0.0f),
		float2(1.0f, 0.0f)
    };
};
struct Particle
{
    float3 InitialPosW : POSITION;
    float3 InitialVelW : VELOCITY;
    float2 SizeW : SIZE;
    float Age : AGE;
    uint Type : TYPE;
};

struct GeoOut
{
    float4 PosH : SV_Position;
    float4 Color : COLOR;
    float2 Pos : TEXCOORD1; // NDC공간의 좌표 값.
    float2 Tex : TEXCOORD2;
};

struct VertexOut
{
    float3 PosW : POSITION;
    float2 SizeW : SIZE;
    float4 Color : COLOR;
    uint Type : TYPE;
};

float3 RandUnitVec3(float offset)
{
    float u = (gGameTime + offset);

	// hlsl 에 랜덤함수가 내장되어 있지 않아 랜덤 텍스쳐를 통해 랜덤 구현.
    float3 v = gRandomTex.SampleLevel(samAnisotropic, u, 0).xyz;

    return normalize(v);
}

#define PT_EMITTER 0
#define PT_FLARE 1

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
        float3 look = normalize(gCameraPosW.xyz - gin[0].PosW);
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
