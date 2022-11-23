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

    float LifeTime : LIFETIME; // Start life time
    float Rotation : ROTATION; // Start Rotation
    float4 Color : COLOR; // Start Color
    float Gravity : GRAVITY; // Start Gravity

    int2 texIndex : TEXINDEX; // Grid_Texture
};


struct StreamOutParticle
{
    float3 InitialPosW : POSITION;
    float3 InitialVelW : VELOCITY; // Start speed
    float2 SizeW : SIZE; // Start size
    float Age : AGE;
    uint Type : TYPE; // 방출기인가
    uint VertexID : VERTEXID;

    float LifeTime : LIFETIME; // Start life time
    float Rotation : ROTATION; // Start Rotation
    float4 Color : COLOR; // Start Color
    float Gravity : GRAVITY; // Start Gravity

    int2 texIndex : TEXINDEX; // Grid_Texture

};
struct VertexOut
{
    float3 PosW : POSITION;
    float2 SizeW : SIZE;
    float4 Color : COLOR;
    uint Type : TYPE;
    float Rotation : ROTATION;
    float2 QuadTexC[4] : QUADTEXCOORD;
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
    vout.LifeTime = vin.LifeTime;
    vout.Rotation = vin.Rotation;
    vout.Color = vin.Color;
    vout.Gravity = vin.Gravity;
    vout.texIndex = vin.texIndex;

    return vout;
}

VertexOut DrawVS(Particle vin)
{
    VertexOut vout;

    float t = vin.Age;

     // ------------------------------- over Life Time -------------------------------------------------------
    
    float s = t / vin.LifeTime; // 선형 보간을 위한 t 값

    float4 color = lerp(gColorOverLifetime.gStartColor, gColorOverLifetime.gEndColor, s);
    
    float3 velocity = lerp(0, gVelocityOverLifetime.gVelocity, s);

    float2 size = lerp(gSizeOverLifetime.gStartSize - gSizeOverLifetime.gStartOffset, gSizeOverLifetime.gEndSize + gSizeOverLifetime.gEndOffset, s);

    float2 rotation = lerp(0, gRotationOverLifetime.gAngularVelocity, s);

    size = clamp(size, 0, 1);

     // ------------------------------- over Life Time -------------------------------------------------------

    float3 gravity = { 0, -vin.Gravity, 0 };
	// 가속도 공식
    vout.PosW = t * (vin.InitialVelW + velocity + (t * gravity)) + vin.InitialPosW;

    vout.Color = vin.Color * color;

    vout.SizeW = vin.SizeW * size;

    vout.Type = vin.Type;

    vout.Rotation = vin.Rotation + rotation;


    float xStep = 1.0f / (float) gTextureSheetAnimation.gGrid_XY.x;
    float yStep = 1.0f / (float) gTextureSheetAnimation.gGrid_XY.y;
    float xMax = xStep * vin.texIndex.x;
    float yMax = yStep * vin.texIndex.y;
    float xMin = xStep * (vin.texIndex.x - 1);
    float yMin = yStep * (vin.texIndex.y - 1);
    vout.QuadTexC[0] = float2(xMin, yMax); // lefttop
    vout.QuadTexC[1] = float2(xMin, yMin); //leftbottom
    vout.QuadTexC[2] = float2(xMax, yMax); // righttop
    vout.QuadTexC[3] = float2(xMax, yMin); // ritghtbottom


    return vout;
}

