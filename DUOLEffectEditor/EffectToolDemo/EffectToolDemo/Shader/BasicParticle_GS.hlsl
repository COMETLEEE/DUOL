#include "ConstantBuffer.hlsli"

Texture1D gRandomTex; // HLSL���� �����Լ��� ����Ǿ� ���� �ʾƼ� ���� �ؽ�ó�� ����� ���� ����

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
    float2 Pos : TEXCOORD1; // NDC������ ��ǥ ��.
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

	// hlsl �� �����Լ��� ����Ǿ� ���� �ʾ� ���� �ؽ��ĸ� ���� ���� ����.
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
