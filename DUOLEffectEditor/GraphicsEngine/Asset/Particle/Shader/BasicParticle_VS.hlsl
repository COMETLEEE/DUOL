#include "ConstantBuffer.hlsli"


//***********************************************
// STREAM-OUT TECH                              *
//***********************************************

struct Particle
{
    uint Type : TYPE; // ������ΰ�
    uint VertexID : SV_VertexID;
    
    float3 PosW : POSITION;
    float3 VelW : VELOCITY; // Start speed
    
    float4 SizeW_StartSize : SIZE_STARTSIZE; // Start size
    float4 Age_LifeTime_Rotation_Gravity : AGE_LIFETIME_ROTATION_GRAVITY;
    
    float4 StartColor : STARTCOLOR;
    float4 Color : COLOR; // Start Color

    float2 QuadTexC[4] : QUADTEX;
    float3 InitEmitterPos : EMITTERPOS;
    float3 PrevPos[15] : PREVPOS; // Trail�� �׸��� ���� ���� �Ÿ����� ��ġ�� ����Ѵ�. 
    
    float3 LatestPrevPos : LASTESTPREVPOS;
};

StreamOutParticle StreamOutVS(Particle vin)
{
    StreamOutParticle vout;
    vout.PosW = vin.PosW;
    vout.VelW = vin.VelW;
    vout.SizeW_StartSize = vin.SizeW_StartSize;
    vout.Age_LifeTime_Rotation_Gravity = vin.Age_LifeTime_Rotation_Gravity;
    vout.Type = vin.Type;
    vout.VertexID = vin.VertexID;
    vout.StartColor = vin.StartColor;
    vout.Color = vin.Color;
    vout.QuadTexC = vin.QuadTexC;
    vout.InitEmitterPos = vin.InitEmitterPos;
    vout.PrevPos = vin.PrevPos;
    vout.LatestPrevPos = vin.LatestPrevPos;
    
    return vout;
}

ParticleVertexOut DrawVS(Particle vin)
{

    ParticleVertexOut vout;

    vout.PosW = vin.PosW;

    vout.LatestPrevPos = vin.LatestPrevPos;
    
    vout.SizeW = vin.SizeW_StartSize.xy;

    vout.Color = vin.Color;

    vout.Type = vin.Type;

    vout.Age_LifeTime_Rotation = vin.Age_LifeTime_Rotation_Gravity.xyz;
    
    vout.QuadTexC = vin.QuadTexC;

    vout.PrevPos = vin.PrevPos; // Ʈ���� ����Ʈ ������ �Ѱܾ� ��.
    
    return vout;
}



struct vertexID
{
    uint index : SV_VertexID; // ������ΰ�
};

struct VertexIDIn
{
    uint index : SV_VertexID; // ������ΰ�
};
VertexIDOut ComputeShaderDrawVS(VertexIDIn vin)
{
    VertexIDOut vout;
    
    vout.index = vin.index;
    
    return vout;
}


//ParticleVertexOut ComputeShaderDrawVS(vertexID vin)
//{
//    ParticleVertexOut vout;
    
//    Particle p = particleBuffer[vin.index];
    
//    vout.PosW = p.PosW;

//    vout.LatestPrevPos = p.LatestPrevPos;
    
//    vout.SizeW = p.SizeW_StartSize.xy;

//    vout.Color = p.Color;

//    vout.Type = p.Type;

//    vout.Age_LifeTime_Rotation = p.Age_LifeTime_Rotation_Gravity.xyz;
    
//    vout.QuadTexC = p.QuadTexC;

//    vout.PrevPos = p.PrevPos; // Ʈ���� ����Ʈ ������ �Ѱܾ� ��.
    
    
//    return vout;
//}