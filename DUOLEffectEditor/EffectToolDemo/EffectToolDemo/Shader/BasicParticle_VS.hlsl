#include "ConstantBuffer.hlsli"


//***********************************************
// STREAM-OUT TECH                              *
//***********************************************

#define PT_EMITTER 0
#define PT_FLARE 1

extern int test = 0;

struct Particle
{
    float3 InitialPosW : POSITION;
    float3 InitialVelW : VELOCITY;
    float2 SizeW : SIZE;
    float Age : AGE;
    uint Type : TYPE;
};

struct VertexOut
{
    float3 PosW : POSITION;
    float2 SizeW : SIZE;
    float4 Color : COLOR;
    uint Type : TYPE;
};

Particle StreamOutVS(Particle vin)
{
    test += 1; /// �ذ��� �����غ���~~~!@!!
    return vin;
}

VertexOut DrawVS(Particle vin)
{
    VertexOut vout;

    float t = vin.Age;
    float3 gravity = { 0, -gGravityModifier[0], 0 };
	// ���ӵ� ����
    vout.PosW = 0.5f * t * t * gravity + t * vin.InitialVelW + vin.InitialPosW;

	// �ð��� �������� ���� ��������
    //float opacity = 1.0f - smoothstep(0.0f, 1.0f, t / 1.0f);
    vout.Color = gStartColor[0];

    vout.SizeW = vin.SizeW;


    vout.Type = vin.Type;
    //clip(-1);
    return vout;
}

