#include "ConstantBuffer.hlsli"


//***********************************************
// STREAM-OUT TECH                              *
//***********************************************

#define PT_EMITTER 0
#define PT_FLARE 1

struct Particle
{
    float3 InitialPosW : POSITION;
    float3 InitialVelW : VELOCITY;
    float2 SizeW : SIZE;
    float Age : AGE;
    uint Type : TYPE;
    uint VertexID : SV_VertexID;
};


struct StreamOutParticle
{
    float3 InitialPosW : POSITION;
    float3 InitialVelW : VELOCITY;
    float2 SizeW : SIZE;
    float Age : AGE;
    uint Type : TYPE;
    uint VertexID : VERTEXID;

};
struct VertexOut
{
    float3 PosW : POSITION;
    float2 SizeW : SIZE;
    float4 Color : COLOR;
    uint Type : TYPE;
    float Rotation : ROTATION;
};

StreamOutParticle StreamOutVS(Particle vin)
{
    StreamOutParticle vout;
    vout.InitialPosW = vin.InitialPosW;
    vout.InitialVelW = vin.InitialVelW;
    vout.SizeW = vin.SizeW;
    vout.Age = vin.Age;
    vout.Type = vin.Type;
    vout.VertexID = vin.VertexID;
    return vout;
}

VertexOut DrawVS(Particle vin)
{
    VertexOut vout;

    float t = vin.Age;

     // ------------------------------- over Life Time -------------------------------------------------------
    
    float s = t / gCommonInfo.gStartLifeTime; // 선형 보간을 위한 t 값

    float4 color = lerp(gColorOverLifetime.gStartColor, gColorOverLifetime.gEndColor, s);
    
    float3 velocity = lerp(0, gVelocityOverLifetime.gVelocity, s);

    float2 size = lerp(gSizeOverLifetime.gStartSize - gSizeOverLifetime.gStartOffset, gSizeOverLifetime.gEndSize + gSizeOverLifetime.gEndOffset, s);

    float2 rotation = lerp(0, gRotationOverLifetime.gAngularVelocity, s);

    size = clamp(size, 0, 1);

     // ------------------------------- over Life Time -------------------------------------------------------

    float3 gravity = { 0, -gCommonInfo.gGravityModifier[0], 0 };
	// 가속도 공식
    vout.PosW =  t * t * gravity + t * (vin.InitialVelW + velocity) + vin.InitialPosW;

    vout.Color = gCommonInfo.gStartColor[0] * color;

    vout.SizeW = vin.SizeW * size;

    vout.Type = vin.Type;

    vout.Rotation = rotation;

    return vout;
}

