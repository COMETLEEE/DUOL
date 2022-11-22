#include "ConstantBuffer.hlsli"

Texture1D gRandomTex : register(t0); // HLSL���� �����Լ��� ����Ǿ� ���� �ʾƼ� ���� �ؽ�ó�� ����� ���� ����

struct StreamOutParticle
{
    float3 InitialPosW : POSITION;
    float3 InitialVelW : VELOCITY;
    float2 SizeW : SIZE;
    float Age : AGE;
    uint Type : TYPE;
    uint VertexID : VERTEXID;

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

static const float2 gQuadTexC[4] =
{
    float2(0.0f, 1.0f),
		float2(1.0f, 1.0f),
		float2(0.0f, 0.0f),
		float2(1.0f, 0.0f)
};


float3 RandUnitVec3(float offset)
{
    float u = (gCommonInfo.gParticlePlayTime + offset);

	// hlsl �� �����Լ��� ����Ǿ� ���� �ʾ� ���� �ؽ��ĸ� ���� ���� ����.
    float3 v = gRandomTex.SampleLevel(samAnisotropic, u, 0).xyz;

    return normalize(v);
}

#define PT_EMITTER 0
#define PT_FLARE 1

[maxvertexcount(2)]
void StreamOutGS(point StreamOutParticle gin[1],
	inout PointStream<StreamOutParticle> ptStream)
{
    gin[0].Age += gTimeStep;

    if (gin[0].Type == PT_EMITTER) // 0�̶�� �����.
    {
        if (gin[0].VertexID < gCommonInfo.gMaxParticles)
        {
            if (gCommonInfo.gDuration >= gCommonInfo.gParticlePlayTime || gCommonInfo.gisLooping)
            {
		// ���� �ð����� ����
                if (gin[0].Age > gEmission.gEmissiveTime)
                {
                    //for (int i = 0; i < gEmissiveCount; i++)
                    //{
                    float3 vRandom = RandUnitVec3(gin[0].VertexID * 0.003f);
                    vRandom *= 0.5f;

                    StreamOutParticle p;
                    p.InitialPosW = gCommonInfo.gEmitPosW.xyz;
                    p.InitialVelW = gCommonInfo.gStartSpeed[0] * vRandom;
                    p.SizeW = float2(3.0f, 3.0f);
                    p.Age = 0.0f;
                    p.Type = PT_FLARE;
                    p.VertexID = 0;

                    ptStream.Append(p);
			// ���� �ð����� ���ο� ���ؽ� ����.
			// �ð� ����
                    gin[0].Age = 0.0f;
                    //}
                }
            }
        }
    	// �׻� ������ ������Ų��.
        ptStream.Append(gin[0]);
        // ����Ⱑ �Ѱ��� ������ ������ �������� �Ǹ� �з��� �������� ���װ� �ִ�... �̰��� ��� �ذ� �� ���ΰ�...?
    }
    else
    {
		// ��ƼŬ�� �����ð�
        if (gin[0].Age <= gCommonInfo.gStartLifeTime[0])
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

        float costheta = cos(gCommonInfo.gStartRotation);
        float sintheta = sin(gCommonInfo.gStartRotation);
        float OneMinusCos = 1.0f - costheta;
        
        float X2 = pow(look.x, 2);
        float Y2 = pow(look.y, 2);
        float Z2 = pow(look.z, 2);
        
        float4x4 RotationTM;
        
        RotationTM[0] = float4(
        X2 * OneMinusCos + costheta,
        look.x * look.y * OneMinusCos + look.z * sintheta,
        look.x * look.z * OneMinusCos - look.y * sintheta,
        0);
        RotationTM[1] = float4(
        look.x * look.y * OneMinusCos - look.z * sintheta,
                Y2 * OneMinusCos + costheta,
        look.y * look.z * OneMinusCos + look.x * sintheta,
        0);
        RotationTM[2] = float4(
        look.x * look.z * OneMinusCos + look.y * sintheta,
        look.y * look.z * OneMinusCos - look.x * sintheta,
                Z2 * OneMinusCos + costheta,
        0);
        RotationTM[3] = float4(0, 0, 0, 1);

        right = mul(float4(right, 1.0f), RotationTM);
        up = mul(float4(up, 1.0f), RotationTM);
        

        
        float halfWidth = 0.5f * gCommonInfo.gStartSize[0];
        float halfHeight = 0.5f * gCommonInfo.gStartSize[1];

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
