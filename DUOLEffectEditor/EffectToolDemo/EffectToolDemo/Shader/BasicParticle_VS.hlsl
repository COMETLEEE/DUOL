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
    test += 1; /// 해결방안 생각해보자~~~!@!!
    return vin;
}

VertexOut DrawVS(Particle vin)
{
    VertexOut vout;

    float t = vin.Age;
    float3 gravity = { 0, -gGravityModifier[0], 0 };
	// 가속도 공식
    vout.PosW = 0.5f * t * t * gravity + t * vin.InitialVelW + vin.InitialPosW;

	// 시간이 지날수록 색이 옅어지게
    //float opacity = 1.0f - smoothstep(0.0f, 1.0f, t / 1.0f);
    vout.Color = gStartColor[0];

    vout.SizeW = vin.SizeW;


    vout.Type = vin.Type;
    //clip(-1);
    return vout;
}

